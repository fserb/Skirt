import {Vec3, Ray} from "./utils.js";
import {Sphere, HitList, Camera} from "./raytracer.js";

// eslint-disable-next-line no-unused-vars
let WIDTH, HEIGHT, THREADS, ID, SAMPLING;
let FIRST = true;

// eslint-disable-next-line no-unused-vars
function log(...msg) {
  if (ID != 0 || !FIRST) return;
  self.console.log(...msg);
}

function setup() {
}

function randomInUnitSphere() {
  let p;
  do {
    p = new Vec3(2 * Math.random() - 1,
      2 * Math.random() - 1, 2 * Math.random() - 1);
  } while (p.sqlen >= 1.0);
  return p;
}

function color(r, world) {
  const h = world.hit(r, 0.001, Infinity);
  if (h) {
    const target = h.p.add(h.normal).add(randomInUnitSphere());
    return color(new Ray(h.p, target.sub(h.p)), world).muls(0.5);
  }

  const unitDirection = r.direction.unit();
  const f = 0.5 * (unitDirection.y + 1.0);
  const a = new Vec3(1, 1, 1);
  const b = new Vec3(0.5, 0.7, 1.0);
  return a.muls(1.0 - f).add(b.muls(f));
}

function render(data, info) {
  const world = new HitList();
  world.push(new Sphere(new Vec3(0, 0, -1), 0.5));
  world.push(new Sphere(new Vec3(0, -100.5, -1), 100));

  const camera = new Camera();

  let p = 0;
  for (let j = info.height - 1; j >= 0; --j) {
    for (let i = 0; i < info.width; ++i) {
      let col = new Vec3(0, 0, 0);

      for (let s = 0; s < SAMPLING; ++s) {
        const x = info.x + i;
        const y = HEIGHT - info.y - info.height + j;
        const u = (x + Math.random()) / WIDTH;
        const v = (y + Math.random()) / HEIGHT;

        const r = camera.getRay(u, v);
        col = col.add(color(r, world));
      }

      col = col.divs(SAMPLING);

      data[p++] = Math.sqrt(col.r) * 255;
      data[p++] = Math.sqrt(col.g) * 255;
      data[p++] = Math.sqrt(col.b) * 255;
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
    SAMPLING = ev.data.sampling;
    setup();
  } else if (ev.data.type == 'work') {
    work(ev.data);
    FIRST = false;
  }
});
