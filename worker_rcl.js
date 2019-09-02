import RCLModule from "./rcl/rcl.mjs";

// eslint-disable-next-line no-unused-vars
let WIDTH, HEIGHT, THREADS, ID, RNG, RCL;
const LOADING = [];

function work(input) {
  const info = input.info;
  const block = info.block;

  const b = RCL.ccall("render", "unsigned char*",
    ["int", "int", "int", "int", "int"],
    [block.x, block.y, block.width, block.height, block.sampling]);

  const size = block.width * block.height * 4;
  const data = new Uint8Array(size);
  for (let i = 0; i < size; ++i) {
    data[i] = RCL.HEAPU8[b + i];
  }
  RCL._free(b);
  self.d = data;
  info.data = data.buffer;
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
    RNG = ev.data.seedrandom;
    RCL = RCLModule();
    LOADING.push(new Promise((acc, rej) => {
      RCL.then(_ => {
        self.RCL = RCL;
        RCL.ccall("setup", "void", ["int", "int", "string"], [WIDTH, HEIGHT, RNG]);
        acc();
      });
    }));
  } else if (ev.data.type == 'work') {
    if (LOADING) {
      Promise.all(LOADING).then(() => {
        work(ev.data);
      });
    } else {
      work(ev.data);
    }
  }
});
