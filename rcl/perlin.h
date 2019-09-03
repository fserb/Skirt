#ifndef __PERLIN_H__
#define __PERLIN_H__

#include <cmath>
#include "utils.h"

using namespace std;

void permute(int* ret, int n) {
  for (int i = n - 1; i >= 0; --i) {
    int target = floor(frand() * (i + 1));
    swap(ret[i], ret[target]);
  }
}

vec3* generatePerlin() {
  vec3* ret = new vec3[256];
  for (int i = 0; i < 256; ++i) {
    ret[i] = unit(-1 + (2 * vec3(frand(), frand(), frand())));
  }
  return ret;
}

int* generatePerm() {
  int* ret = new int[256];
  for (int i = 0; i < 256; ++i) {
    ret[i] = i;
  }
  permute(ret, 256);
  return ret;
}

float interpTrilinear(vec3 c[2][2][2], float u, float v, float w) {
  float uu = u * u * (3 - 2 * u);
  float vv = v * v * (3 - 2 * v);
  float ww = w * w * (3 - 2 * w);
  float ret = 0;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        vec3 weight(u - i, v - j, w - k);
        ret += (i * uu + (1 - i) * (1 - uu)) *
               (j * vv + (1 - j) * (1 - vv)) *
               (k * ww + (1 - k) * (1 - ww)) *
               dot(c[i][j][k], weight);
      }
    }
  }
  return ret;
}

class Perlin {
public:
  Perlin() {
    ranvec = generatePerlin();
    permX = generatePerm();
    permY = generatePerm();
    permZ = generatePerm();
  }

  ~Perlin() {
    delete[] ranvec;
    delete[] permX;
    delete[] permY;
    delete[] permZ;
  }

  float noise(const vec3& p) const {
    float u = p.x() - floor(p.x());
    float v = p.y() - floor(p.y());
    float w = p.z() - floor(p.z());
    int i = floor(p.x());
    int j = floor(p.y());
    int k = floor(p.z());
    vec3 c[2][2][2];
    for (int di = 0; di < 2; di++) {
      for (int dj = 0; dj < 2; dj++) {
        for (int dk = 0; dk < 2; dk++) {
          c[di][dj][dk] = ranvec[
            permX[(i+di) & 255] ^
            permY[(j+dj) & 255] ^
            permZ[(k+dk) & 255]];
        }
      }
    }
    return interpTrilinear(c, u, v, w);
  }

  float turb(const vec3& p, int depth=7) const {
    float accum = 0;
    vec3 temp = p;
    float weight = 1.0;
    for (int i = 0; i < depth; ++i) {
      accum += weight * noise(temp);
      weight *= 0.5;
      temp *= 2;
    }
    return fabs(accum);
  }

  int* permX;
  int* permY;
  int* permZ;
  vec3* ranvec;
};

#endif
