import {v3} from "./utils.js";

import setup from "./scene.js";

import "./node_modules/seedrandom/seedrandom.js";

// eslint-disable-next-line no-unused-vars
let WIDTH, HEIGHT, THREADS, ID, RNG;

const MAX_COLOR_DEPTH = 50;

const LOADING = [];

let FIRST = true;
// eslint-disable-next-line no-unused-vars
self.log = function(...msg) {
  if (ID != 0 || !FIRST) return;
  self.console.log(...msg);
};

let world, camera;

function color(ray, world, depth) {
  const hit = world.hit(ray, 0.001, Infinity);
  if (hit) {
    const emitted = hit.material.emitted(hit.u, hit.v, hit.p);
    const mathit = hit.material.scatter(ray, hit);
    if (depth < MAX_COLOR_DEPTH && mathit) {
      return v3.add(emitted,
        v3.mul(color(mathit.scattered, world, depth + 1),
          mathit.attenuation));
    } else {
      return emitted;
    }
  }
  return v3.new(0, 0, 0);
}

function render(data, info) {
  const sampling = Math.max(1, info.sampling);

  let p = 0;
  for (let j = info.height - 1; j >= 0; --j) {
    for (let i = 0; i < info.width; ++i) {
      let col = v3.new(0, 0, 0);
      const x = info.x + i;
      const y = HEIGHT - info.y - info.height + j;

      for (let s = 0; s < sampling; ++s) {
        const u = (x + Math.random()) / WIDTH;
        const v = (y + Math.random()) / HEIGHT;
        const r = camera.getRay(u, v);
        col = v3.add(col, color(r, world, 0));
      }
      col = v3.div(col, sampling);

      data[p++] = Math.sqrt(col.r) * 255;
      data[p++] = Math.sqrt(col.g) * 255;
      data[p++] = Math.sqrt(col.b) * 255;
      data[p++] = 255;
    }
  }
}

function work(input) {
  const info = input.info;
  const block = info.block;
  const buffer = new ArrayBuffer(block.width * block.height * 4);
  const data = new Uint8ClampedArray(buffer);
  render(data, block);
  info.data = buffer;
  info.id = input.id;
  info.codeHash = input.codeHash;
  postMessage(info, [info.data]);
}

self.addEventListener('message', ev => {
  if (ev.data.type == 'init') {
    ID = ev.data.id;
    THREADS = ev.data.threads;
    WIDTH = ev.data.width;
    HEIGHT = ev.data.height;
    RNG = new Math.seedrandom(ev.data.seedrandom);
    let loading;
    [world, camera, loading] = setup(WIDTH, HEIGHT, RNG);
    if (loading) {
      for (let i = 0; i < loading.length; ++i) LOADING.push(loading[i]);
    }
  } else if (ev.data.type == 'work') {
    if (LOADING) {
      Promise.all(LOADING).then(() => {
        work(ev.data);
      });
    } else {
      work(ev.data);
      FIRST = false;
    }
  }
});
