#include <emscripten.h>

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cfloat>
#include <chrono>

#include "utils.h"
#include "vec3.h"
#include "raytracer.h"
#include "objects.h"
#include "texture.h"

#include "scene.h"

using namespace std;

int WIDTH, HEIGHT;
const int MAX_DEPTH = 20;
Scene scene;

vec3 color(const Ray& r, Hitable& world, int depth=0) {
  Hit h = world.hit(r, 0.001, FLT_MAX);
  if (h) {
    Scatter s = h.material->scatter(r, h);
    vec3 emitted = h.material->emitted(h.uv, h.p);

    if (depth < MAX_DEPTH && s) {
      return emitted + s.attenuation * color(s.scattered, world, depth + 1);
    } else {
      return emitted;
    }
  }

  return vec3(0, 0, 0);
  // vec3 dir = unit(r.direction);
  // float t = 0.5 * (dir.y() + 1.0);
  // return (1.0 - t) * vec3(1., 1., 1.) + t * vec3(0.5, 0.7, 1.0);
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
void setup(int width, int height, char* seedrandom) {
  WIDTH = width;
  HEIGHT = height;

  int sr = 0;
  while(*seedrandom) sr *= *seedrandom++;

  srand(sr);
  scene = sceneWeek(WIDTH, HEIGHT);

  srand(static_cast <unsigned> (time(0)));
}

EMSCRIPTEN_KEEPALIVE
unsigned char* render(int x0, int y0, int width, int height, int sampling) {
  auto start_time = std::chrono::high_resolution_clock::now();

  sampling = max(1, sampling);
  unsigned char* ret = (unsigned char*)malloc(width * height * 4);

  int p = 0;
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      int x = x0 + i;
      int y = HEIGHT - y0 - height + j;

      vec3 col(0, 0, 0);
      for (int s = 0; s < sampling; ++s) {
        float u = float(x + frand()) / WIDTH;
        float v = float(y + frand()) / HEIGHT;

        Ray r = scene.camera.getRay(u, v);
        col += color(r, *scene.world);
      }

      col /= float(sampling);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

      ret[p++] = max(0, min(255, int(255.99 * col[0])));
      ret[p++] = max(0, min(255, int(255.99 * col[1])));
      ret[p++] = max(0, min(255, int(255.99 * col[2])));
      ret[p++] = 255;
    }

  }
  assert(p == width * height * 4);

  auto end_time = std::chrono::high_resolution_clock::now();
  auto time = end_time - start_time;

  double t = (time / std::chrono::microseconds(1)) / 1000.0;
  double ratio = width * height * sampling / t;

  // printf("Block took %.1fs or %.1f samples/ms\n", t / 1000.0, ratio);

  return ret;
}

} // extern

