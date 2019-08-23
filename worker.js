import {Vec3, Ray} from "./utils.js";
import {Sphere, HitList, Camera,
  Lambertian, Metal, Dielectric} from "./raytracer.js";

// eslint-disable-next-line no-unused-vars
let WIDTH, HEIGHT, THREADS, ID, RANDPOOL;

let FIRST = true;
// eslint-disable-next-line no-unused-vars
self.log = function(...msg) {
  if (ID != 0 || !FIRST) return;
  self.console.log(...msg);
};

let _Rcount = -1;
self.R = function() {
  _Rcount = (_Rcount + 1) % RANDPOOL.length;
  return RANDPOOL[_Rcount];
};

let world, camera;

function setup1() {
  world = new HitList();

  world.push(new Sphere(new Vec3(0, 0, -1), 0.5,
    new Lambertian(new Vec3(0.1, 0.2, 0.5))));
  world.push(new Sphere(new Vec3(0, -100.5, -1), 100,
    new Lambertian(new Vec3(0.8, 0.8, 0.0))));

  world.push(new Sphere(new Vec3(1, 0, -1), 0.5,
    new Metal(new Vec3(0.8, 0.6, 0.2), 0.2)));

  world.push(new Sphere(new Vec3(-1, 0, -1), 0.5,
    new Dielectric(1.5)));
  world.push(new Sphere(new Vec3(-1, 0, -1), -0.45,
    new Dielectric(1.5)));

  const lookfrom = new Vec3(3, 3, 2);
  const lookat = new Vec3(0, 0, -1);
  const distToFocus = lookfrom.sub(lookat).len;

  camera = new Camera(
    lookfrom, lookat,
    new Vec3(0, 1, 0),
    20, WIDTH / HEIGHT, 2.0, distToFocus);
}

function setup() {
  world = new HitList();

  world.push(new Sphere(new Vec3(0, -1000, 0), 1000,
    new Lambertian(new Vec3(0.5, 0.5, 0.5))));

  for (let a = -11; a < 11; ++a) {
    for (let b = -11; b < 11; ++b) {
      const mat = R();
      const center = new Vec3(a + 0.9 * R(),
        0.2, b + 0.9 * R());
      if (center.sub(new Vec3(4, 0.2, 0)).len < 0.9) continue;

      if (mat < 0.8) {
        world.push(new Sphere(center, 0.2,
          new Lambertian(new Vec3(
            R() * R(),
            R() * R(),
            R() * R()))));
      } else if (mat < 0.95) {
        world.push(new Sphere(center, 0.2, new Metal(new Vec3(
          0.5 * (1 + R()),
          0.5 * (1 + R()),
          0.5 * (1 + R())),
          0.5 * R())));
      } else {
        world.push(new Sphere(center, 0.2, new Dielectric(1.5)));
      }
    }
  }

  world.push(new Sphere(new Vec3(0, 1, 0), 1, new Dielectric(1.5)));
  world.push(new Sphere(new Vec3(-4, 1, 0), 1, new Lambertian(new Vec3(0.4, 0.2, 0.1))));
  world.push(new Sphere(new Vec3(4, 1, 0), 1, new Metal(new Vec3(0.7, 0.6, 0.5), 0.0)));

  const lookfrom = new Vec3(13, 2, 3);
  const lookat = new Vec3(0, 0, -1);
  const distToFocus = 10.0; //lookfrom.sub(lookat).len;
  const aperture = 0.1;

  camera = new Camera(
    lookfrom, lookat,
    new Vec3(0, 1, 0),
    20, WIDTH / HEIGHT, aperture, distToFocus);
}

function color(ray, world, depth) {
  const hit = world.hit(ray, 0.001, Infinity);
  if (hit) {
    const mathit = hit.material.scatter(ray, hit);
    if (depth < 50 && mathit) {
      return color(mathit.scattered, world, depth + 1).mul(mathit.attenuation);
    } else {
      return new Vec3(0, 0, 0);
    }
  }

  const unitDirection = ray.direction.unit();
  const f = 0.5 * (unitDirection.y + 1.0);
  const a = new Vec3(1, 1, 1);
  const b = new Vec3(0.5, 0.7, 1.0);
  return a.muls(1.0 - f).add(b.muls(f));
}

function render(data, info) {
  const sampling = Math.max(1, info.sampling);

  let p = 0;
  for (let j = info.height - 1; j >= 0; --j) {
    for (let i = 0; i < info.width; ++i) {
      let col = new Vec3(0, 0, 0);
      const x = info.x + i;
      const y = HEIGHT - info.y - info.height + j;

      for (let s = 0; s < sampling; ++s) {
        const u = (x + Math.random()) / WIDTH;
        const v = (y + Math.random()) / HEIGHT;
        const r = camera.getRay(u, v);
        col = col.add(color(r, world, 0));
      }
      col = col.divs(sampling);

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
