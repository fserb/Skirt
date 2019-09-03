import {v3} from "./utils.js";
import {BVH, HitList, Camera} from "./raytracer.js";
import {FlipNormal, Sphere, RectXY, RectXZ, RectYZ,
  Box, Transform, ConstantMedium} from "./object.js";
import {Lambertian, Metal, Dielectric,
  ConstantTexture, CheckerTexture, NoiseTexture, ImageTexture,
  DiffuseLight
} from "./texture.js";

export default function(width, height, _RNG) {
  const world = new HitList();


  world.push(new Sphere(v3.new(0, 0, -1), 0.5, new Lambertian(new ConstantTexture(0.8, 0.3, 0.3))));
  world.push(new Sphere(v3.new(0, -100.5, -1), 100, new Lambertian(new ConstantTexture(0.8, 0.8, 0.0))));
  world.push(new Sphere(v3.new(1, 0, -1), 0.5, new Metal(new ConstantTexture(0.8, 0.6, 0.2))));
  world.push(new Sphere(v3.new(-1, 0, -1), 0.5, new Metal(new ConstantTexture(0.8, 0.8, 0.8))));


  const lookfrom = v3.new(3, 3, 2);
  const lookat = v3.new(0, 0, -1);
  const distToFocus = v3.sub(lookfrom, lookat).len;

  const camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    20, width / height, 2.0, distToFocus);

  return [world, camera];
}


function _setup1(width, height, _RNG) {
  const world = new HitList();

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

  const camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    20, width / height, 2.0, distToFocus);

  return [world, camera];
}

function _setup2(width, height, RNG) {
  let world = new HitList();

  const check = new CheckerTexture(
    new ConstantTexture(0.2, 0.3, 0.1),
    new ConstantTexture(0.9, 0.9, 0.9));

  world.push(new Sphere(v3.new(0, -1000, 0), 1000,
    new Lambertian(check)));

  for (let a = -11; a < 11; ++a) {
    for (let b = -11; b < 11; ++b) {
      const mat = RNG();
      const center = v3.new(a + 0.9 * RNG(),
        0.2, b + 0.9 * RNG());
      if (v3.sub(center, v3.new(4, 0.2, 0)).len < 0.9) continue;

      if (mat < 0.8) {
        world.push(new Sphere(center, 0.2,
          new Lambertian(new ConstantTexture(
            RNG() * RNG(),
            RNG() * RNG(),
            RNG() * RNG()))));
      } else if (mat < 0.95) {
        world.push(new Sphere(center, 0.2, new Metal(new ConstantTexture(
          0.5 * (1 + RNG()),
          0.5 * (1 + RNG()),
          0.5 * (1 + RNG())),
        0.5 * RNG())));
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

  const camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    20, width / height, aperture, distToFocus);

  return [world, camera];
}

function _setup3(width, height, RNG) {
  let world = new HitList();

  const pertext = new NoiseTexture(RNG, 5);

  const earth = new ImageTexture("./textures/2k_earth_daymap.jpg");
  const loading = [];
  loading.push(earth.load);

  const light = new DiffuseLight(new ConstantTexture(5, 5, 5));

  world.push(new Sphere(v3.new(0, -1000, 0), 1000, new Lambertian(pertext)));
  world.push(new Sphere(v3.new(0, 2, 0), 2, new Lambertian(earth)));

  world.push(new Sphere(v3.new(2, 1, 3), 0.5, light));

  world = new BVH(world);

  const lookfrom = v3.new(13, 2, 3);
  const lookat = v3.new(0, 0, 0);
  const distToFocus = 10.0;
  const aperture = 0.0;

  const camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    20, width / height, aperture, distToFocus);

  return [world, camera, loading];
}

function _setup(width, height, RNG) {
  let world = new HitList();

  const pertext = new NoiseTexture(RNG, 4);

  world.push(new Sphere(v3.new(0, -1000, 0), 1000, new Lambertian(pertext)));
  world.push(new Sphere(v3.new(0, 2, 0), 2, new Lambertian(pertext)));

  world.push(new Sphere(v3.new(0, 7, 0), 2,
    new DiffuseLight(new ConstantTexture(4, 4, 4))));
  world.push(new RectXY(3, 1, 5, 3, -2,
    new DiffuseLight(new ConstantTexture(4, 4, 4))));

  world = new BVH(world);

  const lookfrom = v3.new(13, 2, 2);
  const lookat = v3.new(0, 0, 0);
  const distToFocus = 10.0;
  const aperture = 0.0;
  const vfov = 50;

  const camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    vfov, width / height, aperture, distToFocus);

  return [world, camera];
}

function _setup4(width, height, _RNG) {
  let world = new HitList();

  const red = new Lambertian(new ConstantTexture(0.65, 0.05, 0.05));
  const white = new Lambertian(new ConstantTexture(0.73, 0.73, 0.73));
  const green = new Lambertian(new ConstantTexture(0.12, 0.45, 0.15));
  const light = new DiffuseLight(new ConstantTexture(7, 7, 7));

  world.push(new FlipNormal(new RectYZ(0, 0, 555, 555, 555, green)));
  world.push(new RectYZ(0, 0, 555, 555, 0, red));
  world.push(new RectXZ(123, 147, 423, 412, 554, light));
  world.push(new FlipNormal(new RectXZ(0, 0, 555, 555, 555, white)));
  world.push(new RectXZ(0, 0, 555, 555, 0, white));
  world.push(new FlipNormal(new RectXY(0, 0, 555, 555, 555, white)));

  const box1 =
    new Transform(
      v3.new(130 + 82.5, 82.5, 65 + 82.5),
      v3.new(0, -Math.PI / 6, 0),
      v3.new(82.5, 82.5, 82.5),
      new Box(v3.new(-1, -1, -1), v3.new(1, 1, 1), white));

  const box2 =
    new Transform(
      v3.new(265 + 82.5, 165, 295 + 82.5),
      v3.new(0, Math.PI / 8, 0),
      v3.new(82.5, 165, 82.5),
      new Box(v3.new(-1, -1, -1), v3.new(1, 1, 1), white));

  world.push(new ConstantMedium(box1, 0.01, new ConstantTexture(1, 1, 1)));
  world.push(new ConstantMedium(box2, 0.01, new ConstantTexture(0, 0, 0)));

  world = new BVH(world);

  const lookfrom = v3.new(278, 278, -800);
  const lookat = v3.new(278, 278, 0);
  const distToFocus = 10.0;
  const aperture = 0.0;
  const vfov = 40;

  const camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    vfov, width / height, aperture, distToFocus);

  return [world, camera];
}

function _setup5(width, height, RNG) {
  let world = new HitList();

  const white = new Lambertian(new ConstantTexture(0.73, 0.73, 0.73));
  const ground = new Lambertian(new ConstantTexture(0.48, 0.83, 0.53));

  let boxlist = new HitList();
  for (let i = 0; i < 20; ++i) {
    for (let j = 0; j < 20; ++j) {
      const w = 100;
      const x0 = -1000 + i * w;
      const z0 = -1000 + j * w;
      const y0 = 0;
      const x1 = x0 + w;
      const y1 = 100 * (RNG() + 0.01);
      const z1 = z0 + w;
      boxlist.push(new Box(v3.new(x0, y0, z0), v3.new(x1, y1, z1), ground));
    }
  }

  world.push(boxlist);

  const light = new DiffuseLight(new ConstantTexture(7, 7, 7));
  world.push(new RectXZ(123, 147, 423, 412, 554, light));

  const center = v3.new(400, 400, 200);

  world.push(new Sphere(v3.new(400, 400, 200), 50,
    new Lambertian(new ConstantTexture(0.7, 0.3, 0.1))));
  world.push(new Sphere(v3.new(260, 150, 45), 50, new Dielectric(1.5)));
  world.push(new Sphere(v3.new(0, 150, 145), 50,
    new Metal(new ConstantTexture(0.8, 0.8, 0.9), 10.0)));

  let boundary = new Sphere(v3.new(360, 150, 145), 70, new Dielectric(1.5));
  world.push(boundary);
  world.push(new ConstantMedium(boundary, 0.2, new ConstantTexture(0.2, 0.4, 0.9)));

  boundary = new Sphere(v3.new(0, 0, 0), 5000, new Dielectric(1.5));
  world.push(boundary);
  world.push(new ConstantMedium(boundary, 0.0001, new ConstantTexture(1.0, 1.0, 1.0)));

  const earth = new ImageTexture("./textures/2k_earth_daymap.jpg");
  const loading = [];
  loading.push(earth.load);
  const emat = new Lambertian(earth);
  world.push(new Sphere(v3.new(400, 200, 400), 100, emat));

  const pertext = new NoiseTexture(RNG, 0.1);
  world.push(new Sphere(v3.new(220, 280, 300), 80, new Lambertian(pertext)));

  const boxlist2 = new HitList();
  for (let j = 0; j < 1000; ++j) {
    boxlist2.push(new Sphere(
      v3.new(165 * (RNG() - 0.5), 165 * (RNG() - 0.5), 165 * (RNG() - 0.5)),
      10, white));
  }

  world.push(new Transform(
    v3.new(-100 + 82.5, 270 + 82.5, 395 + 82.5),
    v3.new(0, Math.PI / 6, 0),
    v3.new(1, 1, 1),
    boxlist2));

  world = new BVH(world);

  const lookfrom = v3.new(278, 278, -800);
  const lookat = v3.new(278, 278, 0);
  const distToFocus = 10.0;
  const aperture = 0.0;
  const vfov = 40;

  const camera = new Camera(
    lookfrom, lookat,
    v3.new(0, 1, 0),
    vfov, width / height, aperture, distToFocus);

  return [world, camera];
}
