import BS from "./bs.js";

import "./node_modules/base64-js/base64js.min.js";

const mode = (() => {
  if (location.hash == "#server") return "server";
  if (location.hash == "#client") return "client";
  return "single";
})();

const startTime = performance.now();

const WIDTH = 506 * 2; //1920 / 2;
const HEIGHT = 253 * 2; // 1080 / 2;
const SAMPLING = 10000;
const THREADS = navigator.hardwareConcurrency;
const WORKSIZE = 5;
const SEEDRANDOM = "world";
const WORKERS = [];
let CODEHASH = "";
let bs = null;

const ctx = document.getElementById("c").getContext("2d", {alpha: false});
ctx.canvas.width = WIDTH;
ctx.canvas.height = HEIGHT;

const blocks = [];
const outblocks = {};

function setupWork() {
  for (let y = 0; y < HEIGHT; y += WORKSIZE) {
    for (let x = 0; x < WIDTH; x += WORKSIZE) {
      blocks.push({x: x, y: y, width: Math.min(WIDTH, x + WORKSIZE) - x,
        height: Math.min(HEIGHT, y + WORKSIZE) - y,
        sampling: 0});
      blocks.push({x: x, y: y, width: Math.min(WIDTH, x + WORKSIZE) - x,
        height: Math.min(HEIGHT, y + WORKSIZE) - y,
        sampling: SAMPLING});
    }
  }

  blocks.total = blocks.length / 2;

  blocks.sort((a, b) => {
    if (a.sampling != b.sampling) return a.sampling - b.sampling;
    const xa = (a.x + WORKSIZE / 2 - WIDTH / 2);
    const ya = (a.y + WORKSIZE / 2 - HEIGHT / 2);
    const xb = (b.x + WORKSIZE / 2 - WIDTH / 2);
    const yb = (b.y + WORKSIZE / 2 - HEIGHT / 2);
    const da = Math.sqrt(xa * xa + ya * ya);
    const db = Math.sqrt(xb * xb + yb * yb);
    return da - db;
  });
}

async function clearTube() {
  while (true) {
    let job;
    try {
      job = await bs.peekReady();
    } catch(ex) {
      break;
    }
    try {
      await bs.delete(job.id);
    } catch(ex) { ""; }
  }
}

async function serverSetJobs() {
  console.log("setting jobs");
  await bs.watch("jobs");
  await bs.ignore("default");
  await bs.use("jobs");
  await clearTube();

  await bs.watch("result");
  await bs.ignore("jobs");
  await bs.use("result");
  await clearTube();

  await bs.use("jobs");
  for (let i = 0; i < blocks.length; ++i) {
    bs.put({
      codeHash: CODEHASH,
      block: blocks[i],
      seedrandom: SEEDRANDOM,
    }, i, 0, 60);
  }
  console.log(`Created ${blocks.length} jobs.`);
  blocks.length = 0;
}

async function fetch() {
  try {
    return await bs.reserve(0);
  } catch(ex) {
    return null;
  }
}

async function serverCheck() {
  while (true) {
    const job = await fetch();
    if (!job) break;
    const info = job.data;

    if (info.codeHash != CODEHASH) {
      await bs.delete(job.id);
      continue;
    }

    const p = info.block.x + info.block.y * WIDTH;

    if (!outblocks[p] || outblocks[p] < info.block.sampling) {
      const bytes = self.base64js.toByteArray(info.data);
      const blob = await new Response(bytes).blob();
      const ib = await createImageBitmap(blob);
      ctx.drawImage(ib, info.block.x, info.block.y);
      outblocks[p] = info.block.sampling;
    }
    await bs.delete(job.id);
  }
}

async function clientGetJob() {
  if (mode == "single") {
    const d = blocks.shift();
    if (d) return {id: 0, data: {block: d}};
    return null;
  }

  try {
    const job = await bs.reserve(0);
    if (job.data.codeHash != CODEHASH) {
      console.error("wrong codehash", job.data.codeHash, CODEHASH);
      await bs.release(job.id);
      window.location.reload();
    }
    // check if job.hash is the same as current hash, if not
    // release and reload page.
    return job;
  } catch(ex) {
    return null;
  }
}

/* eslint-disable no-console */

async function updateWorkers() {
  let allFree = true;

  for (let i = 0; i < WORKERS.length; ++i) {
    if (WORKERS[i].state == "busy") {
      allFree = false;
      if (mode == "client") {
        await bs.touch(WORKERS[i].jobid);
      }
      continue;
    }

    const job = await clientGetJob();
    if (job) {
      if (WORKERS[i].state == "loaded") {
        WORKERS[i].postMessage({
          type: "init", id: i, threads: THREADS,
          seedrandom: job.data.seedrandom || SEEDRANDOM,
          width: WIDTH, height: HEIGHT});
        WORKERS[i].state = "free";
      }
      if (job.id != 0) {
        console.log(`started job ${job.id} at worker ${i}`);
      }
      WORKERS[i].postMessage({type: "work", id: job.id, info: job.data});
      WORKERS[i].state = "busy";
      WORKERS[i].jobid = job.id;
      allFree = false;
    }
  }

  if (mode == "single" && allFree) {
    const t = performance.now();
    const delta = (t - startTime) / 1000;
    console.log("Finish time: " + Math.round(delta * 10) / 10 + "s");
  }
}

async function onMessage(ev) {
  ev.target.state = "free";
  const info = ev.data;
  if (mode == "client") {
    const block = info.block;
    const ofc = new OffscreenCanvas(block.width, block.height);
    const c = ofc.getContext("2d");
    const d = c.createImageData(block.width, block.height);
    d.data.set(new Uint8ClampedArray(info.data));
    c.putImageData(d, 0, 0);

    const blob = await ofc.convertToBlob({type: "image/png"});
    const ab = await new Response(blob).arrayBuffer();

    const bytes = self.base64js.fromByteArray(new Uint8Array(ab));
    info.data = bytes;
    info.codeHash = CODEHASH;

    bs.put(info);
    bs.delete(info.id);
  } else {
    const block = info.block;
    const d = ctx.createImageData(block.width, block.height);
    d.data.set(new Uint8ClampedArray(info.data));
    ctx.putImageData(d, block.x, block.y);
  }

  updateWorkers();
}

async function createWorkers() {
  if (mode == "client") {
    await bs.watch("jobs");
    await bs.ignore("default");
    await bs.use("result");
  }

  for (let i = 0; i < THREADS; ++i) {
    // const worker = new Worker('worker.js', {type:"module"});
    const worker = new Worker('worker_rcl.js', {type:"module"});
    worker.addEventListener('message', onMessage);
    worker.state = "loaded";
    WORKERS.push(worker);
  }
}

console.log("mode", mode);

async function main() {
  CODEHASH = await window.genCodehash();
  console.log("code hash", CODEHASH);

  if (mode == "server" || mode == "single") {
    // create blocks.
    setupWork();
  }
  if (mode == "client") {
    console.log("threads", THREADS);
    document.getElementById("c").style.display = "none";
  }
  if (mode == "server" || mode == "client") {
    // init bs;
    bs = new BS("ws://172.16.0.4:5999/127.0.0.1:11300");
    await bs.ready;
  }

  if (mode == "server") {
    // push jobs to bs, clear up blocks
    await serverSetJobs();
    await serverCheck();
    setInterval(async () => {
      await serverCheck();
    }, 1000);
  }

  if (mode == "client" || mode == "single") {
    // spawn workers
    await createWorkers();
    await updateWorkers();
  }

  if (mode == "client") {
    setInterval(async () => {
      await updateWorkers();
    }, 1000);
  }
}

main();

window.stop = function() {
  for (let i = 0; i < WORKERS.length; ++i) {
    WORKERS[i].terminate();
  }
};
