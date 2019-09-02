#ifndef __UTILS_H__
#define __UTILS_H__

#include "vec3.h"

inline float frand() {
  return static_cast<float>(rand()) / static_cast <float> (RAND_MAX);
}

vec3 randomInUnitSphere() {
  vec3 p = unit(vec3(frand(), frand(), frand())) * cbrt(frand());
  return 2 * p - 1;
}

#endif
