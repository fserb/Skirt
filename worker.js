import {v3} from "./utils.js";
import {BVH, Sphere, HitList, Camera,
  Lambertian, Metal, Dielectric,
  ConstantTexture, CheckerTexture} from "./raytracer.js";

// eslint-disable-next-line no-unused-vars
let WIDTH, HEIGHT, THREADS, ID, RANDPOOL;

let FIRST = true;
// eslint-disable-next-line no-unused-vars
self.log = function(...msg) {
  if (ID != 0 || !FIRST) return;
  self.console.log(...msg);
};

let _Rcount = -1;
function R() {
  _Rcount = (_Rcount + 1) % RANDPOOL.length;
  return RANDPOOL[_Rcount];
}

let world, camera;

function _1setup() {
  world = new HitList();

  world.push(new Sphere(v3.new(0, 0, -1), 0.5,
    new Lambertian(new ConstantTexture(0.1, 0.2, 0.5))));
  world.push(new Sphere(v3.new(0, -100.5, -1), 100,
    new Lambertian(new ConstantTexture(0.8, 0.8, 0.0))));

  world.push(new Sphere(v3.new(1, 0, -1), 0.5,
    new Metal(new ConstantTexture(0.8, 0.6, 0.2), 0.2)));

  world.push(new Sphere(v3.new(-1, 0, -1), 0.5,
    new Dielectric(1.5)));
  world.push(new Sphere(v3.new(-1, 0, -1), -0.45,
    new Dielectric(1.5)));

  const lookfrom = v3.new(3, 3, 2);
  const lookat = v3.new(0, 0, -1);
  const distToFocus = v3.sub(lookfrom, lookat).len;

  camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    20, WIDTH / HEIGHT, 2.0, distToFocus);
}

function _2setup() {
  world = new HitList();

  const check = new CheckerTexture(
    new ConstantTexture(0.2, 0.3, 0.1),
    new ConstantTexture(0.9, 0.9, 0.9));

  world.push(new Sphere(v3.new(0, -1000, 0), 1000,
    new Lambertian(check)));

  for (let a = -11; a < 11; ++a) {
    for (let b = -11; b < 11; ++b) {
      const mat = R();
      const center = v3.new(a + 0.9 * R(),
        0.2, b + 0.9 * R());
      if (v3.sub(center, v3.new(4, 0.2, 0)).len < 0.9) continue;

      if (mat < 0.8) {
        world.push(new Sphere(center, 0.2,
          new Lambertian(new ConstantTexture(
            R() * R(),
            R() * R(),
            R() * R()))));
      } else if (mat < 0.95) {
        world.push(new Sphere(center, 0.2, new Metal(new ConstantTexture(
          0.5 * (1 + R()),
          0.5 * (1 + R()),
          0.5 * (1 + R())),
        0.5 * R())));
      } else {
        world.push(new Sphere(center, 0.2, new Dielectric(1.5)));
      }
    }
  }

  world.push(new Sphere(v3.new(0, 1, 0), 1, new Dielectric(1.5)));
  world.push(new Sphere(v3.new(-4, 1, 0), 1,
    new Lambertian(new ConstantTexture(0.4, 0.2, 0.1))));
  world.push(new Sphere(v3.new(4, 1, 0), 1,
    new Metal(new ConstantTexture(0.7, 0.6, 0.5), 0.0)));

  world = new BVH(world);

  const lookfrom = v3.new(13, 2, 3);
  const lookat = v3.new(0, 0, -1);
  const distToFocus = 10.0; //lookfrom.sub(lookat).len;
  const aperture = 0.1;

  camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    20, WIDTH / HEIGHT, aperture, distToFocus);
}

function setup() {
  world = new HitList();

  const check = new CheckerTexture(
    new ConstantTexture(0.2, 0.3, 0.1),
    new ConstantTexture(0.9, 0.9, 0.9));

  world.push(new Sphere(v3.new(0, -10, 0), 10, new Lambertian(check)));
  world.push(new Sphere(v3.new(0, 10, 0), 10, new Lambertian(check)));

  world = new BVH(world);

  const lookfrom = v3.new(13, 2, 3);
  const lookat = v3.new(0, 0, 0);
  const distToFocus = 10.0;
  const aperture = 0.0;

  camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    20, WIDTH / HEIGHT, aperture, distToFocus);
}

function color(ray, world, depth) {
  const hit = world.hit(ray, 0.001, Infinity);
  if (hit) {
    const mathit = hit.material.scatter(ray, hit);
    if (depth < 50 && mathit) {
      return v3.mul(color(mathit.scattered, world, depth + 1),
        mathit.attenuation);
    } else {
      return v3.new(0, 0, 0);
    }
  }

  const unitDirection = v3.unit(ray.direction);
  const f = 0.5 * (unitDirection.y + 1.0);
  const a = v3.new(1, 1, 1);
  const b = v3.new(0.5, 0.7, 1.0);
  return v3.add(v3.mul(1.0 - f, a), v3.mul(f, b));
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
    RANDPOOL = ev.data.randpool;
    setup();
  } else if (ev.data.type == 'work') {
    work(ev.data);
    FIRST = false;
  }
});
