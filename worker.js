import {Vec3, Ray, Sphere, HitList} from "./utils.js";

// eslint-disable-next-line no-unused-vars
let WIDTH, HEIGHT, THREADS, ID;
let FIRST = true;

// eslint-disable-next-line no-unused-vars
function log(...msg) {
  if (ID != 0 || !FIRST) return;
  self.console.log(...msg);
}

function setup() {
}

function color(r, world) {
  const h = world.hit(r, 0, Infinity);
  if (h) {
    return (new Vec3(h.normal.x + 1, h.normal.y + 1, h.normal.z + 1)).muls(0.5);
  }

  const unitDirection = r.direction.unit();
  const f = 0.5 * (unitDirection.y + 1.0);
  const a = new Vec3(1, 1, 1);
  const b = new Vec3(0.5, 0.7, 1.0);
  return a.muls(1.0 - f).add(b.muls(f));
}

function render(data, info) {
  const lowerLeftCorner = new Vec3(-2, -1.125, -1);
  const horizontal = new Vec3(4, 0, 0);
  const vertical = new Vec3(0, 2.25, 0);
  const origin = new Vec3(0, 0, 0);

  const world = new HitList();

  world.push(new Sphere(new Vec3(0, 0, -1), 0.5));
  world.push(new Sphere(new Vec3(0, -100.5, -1), 100));

  let p = 0;
  for (let j = info.height - 1; j >= 0; --j) {
    for (let i = 0; i < info.width; ++i) {
      const u = (info.x + i) / WIDTH;
      const v = (HEIGHT - (info.y + (info.height - j))) / HEIGHT;

      const r = new Ray(origin,
        lowerLeftCorner.add(horizontal.muls(u)).add(vertical.muls(v)));

      const col = color(r, world);

      data[p++] = col.r * 255;
      data[p++] = col.g * 255;
      data[p++] = col.b * 255;
      data[p++] = 255;
    }
  }
}

function work(input) {
  const info = input.info;
  const buffer = new ArrayBuffer(info.width * info.height * 4);
  const data = new Uint8ClampedArray(buffer);
  render(data, info);
  info.data = buffer;
  info.startTime = input.startTime;
  postMessage(info, [info.data]);
}

self.addEventListener('message', ev => {
  if (ev.data.type == 'init') {
    ID = ev.data.id;
    THREADS = ev.data.threads;
    WIDTH = ev.data.width;
    HEIGHT = ev.data.height;
    setup();
  } else if (ev.data.type == 'work') {
    work(ev.data);
    FIRST = false;
  }
});
