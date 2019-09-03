#ifndef __SCENE_H__
#define __SCENE_H__

#include "utils.h"
#include "vec3.h"
#include "raytracer.h"
#include "objects.h"
#include "texture.h"

struct Scene {
  shared_ptr<Hitable> world;
  Camera camera;
};

Scene scene1(int width, int height) {
  HitList world;

  world.add(Sphere::create(vec3(0, 0, -1), 0.5, Lambertian::create(vec3(0.1, 0.2, 0.5))));
  world.add(Sphere::create(vec3(0, -100.5, -1), 100, Lambertian::create(vec3(0.8, 0.8, 0.0))));
  world.add(Sphere::create(vec3(1, 0, -1), 0.5, Metal::create(vec3(0.8, 0.6, 0.2), 0.0)));
  world.add(Sphere::create(vec3(-1, 0, -1), 0.5, Dielectric::create(1.5)));
  world.add(Sphere::create(vec3(-1, 0, -1), -0.45, Dielectric::create(1.5)));

  vec3 lookfrom(3, 3, 2);
  vec3 lookat(0, 0, -1);
  float distToFocus = (lookfrom - lookat).length();
  float aperture = 2.0;

  Camera camera(lookfrom, lookat, vec3(0, 1, 0), 20, width / height, aperture, distToFocus);

  shared_ptr<BVH> bvh(new BVH(world.list));
  return Scene{bvh, camera};
}

Scene scene2(int width, int height) {
  HitList world;

  world.add(Sphere::create(vec3(0, -1000, 0), 1000, Lambertian::create(vec3(0.5, 0.5, 0.5))));

  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      float mat = frand();
      vec3 center(a + 0.9 * frand(), 0.2, b + 0.9 * frand());
      if ((center - vec3(4, 0, 2.0)).length() > 0.9) {
        if (mat < 0.8) {
          world.add(Sphere::create(center, 0.2,
            Lambertian::create(vec3(frand() * frand(), frand() * frand(), frand() * frand()))));
        } else if (mat < 0.95) {
          world.add(Sphere::create(center, 0.2,
            Metal::create(vec3(
              0.5 * (1 + frand()),
              0.5 * (1 + frand()),
              0.5 * (1 + frand())), 0.5 * frand())));
        } else {
          world.add(Sphere::create(center, 0.2, Dielectric::create(1.5)));
        }
      }
    }
  }

  world.add(Sphere::create(vec3(0, 1, 0), 1.0, Dielectric::create(1.5)));
  world.add(Sphere::create(vec3(-4, 1, 0), 1.0, Lambertian::create(vec3(0.4, 0.2, 0.1))));
  world.add(Sphere::create(vec3(4, 1, 0), 1.0, Metal::create(vec3(0.7, 0.6, 0.5), 0.0)));

  vec3 lookfrom(13, 2, 3);
  vec3 lookat(0, 0, -1);
  float distToFocus = 10.0; //(lookfrom - lookat).length();
  float aperture = 0.1;

  Camera camera(lookfrom, lookat, vec3(0, 1, 0), 20, width / height, aperture, distToFocus);

  shared_ptr<BVH> bvh(new BVH(world.list));
  return Scene{bvh, camera};
}


#endif
