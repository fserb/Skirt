#include <emscripten.h>

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>

#include "vec3.h"

using namespace std;

int WIDTH, HEIGHT;

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

  int p = 0;
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      int x = x0 + i;
      int y = HEIGHT - y0 - height + j;

      vec3 col(float(x) / float(WIDTH), float(y) / float(HEIGHT), 0.2);

      ret[p++] = int(255.99*col.x);
      ret[p++] = int(255.99*col.y);
      ret[p++] = int(255.99*col.z);
      ret[p++] = 255;
    }
  }
  return ret;
}

} // extern

