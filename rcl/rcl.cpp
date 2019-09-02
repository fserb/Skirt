#include <emscripten.h>

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cfloat>

#include "vec3.h"
#include "raytracer.h"
#include "objects.h"

using namespace std;

int WIDTH, HEIGHT;

vec3 color(const Ray& r, Hitable& world) {
  Hit h = world.hit(r, 0, FLT_MAX);
  if (h != NoHit) {
    return 0.5*(h.normal + 1);
  }

  vec3 dir = unit(r.direction);
  float t = 0.5 * (dir.y() + 1.0);
  return (1.0 - t) * vec3(1., 1., 1.) + t * vec3(0.5, 0.7, 1.0);
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
void setup(int width, int height, string seedrandom) {
  WIDTH = width;
  HEIGHT = height;
}

EMSCRIPTEN_KEEPALIVE
unsigned char* render(int x0, int y0, int width, int height, int sampling) {
  sampling = max(1, sampling);
  unsigned char* ret = (unsigned char*)malloc(width * height * 4);

  vec3 llc(-2., -1., -1.);
  vec3 hor(4., 0., 0.);
  vec3 ver(0., 2., 0.);
  vec3 origin(0., 0., 0.);

  HitList world;

  world.add(Sphere::create(vec3(0, 0, -1), 0.5));
  world.add(Sphere::create(vec3(0, -100.5, -1), 100));

  int p = 0;
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      int x = x0 + i;
      int y = HEIGHT - y0 - height + j;

      float u = float(x) / WIDTH;
      float v = float(y) / HEIGHT;

      Ray r(origin, llc + u * hor + v * ver);
      vec3 col = color(r, world);

      ret[p++] = int(255.99 * col.x());
      ret[p++] = int(255.99 * col.y());
      ret[p++] = int(255.99 * col.z());
      ret[p++] = 255;
    }

  }
  assert(p == width * height * 4);
  return ret;
}

} // extern

