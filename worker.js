import {Vec3} from "./utils.js";

let WIDTH, HEIGHT, THREADS, ID;

self.addEventListener('message', ev => {
  if (ev.data.type == 'init') {
    ID = ev.data.id;
    THREADS = ev.data.threads;
    WIDTH = ev.data.width;
    HEIGHT = ev.data.height;
  } else if (ev.data.type == 'work') {
    work(ev.data.info);
  }
});


function work(info) {
  var buffer = new ArrayBuffer(info.width * info.height * 4);
  var data = new Uint8ClampedArray(buffer);
  let p = 0;
  for (let j = info.height - 1; j >= 0; --j) {
    for (let i = 0; i < info.width; ++i) {
      const x = info.x + i;
      const y = HEIGHT - (info.y + (info.height - j));

      const col = new Vec3(x / WIDTH, y / HEIGHT, 0.2);

      data[p++] = col.r * 255;
      data[p++] = col.g * 255;
      data[p++] = col.b * 255;
      data[p++] = 255;
    }
  }

  info.data = buffer;
  postMessage(info, [info.data]);
}
