#ifndef __RAY_H__
#define __RAY_H__

#include <emscripten.h>

#include "vec3.h"

class Ray {
public:
  Ray() {}
  Ray(const vec3& origin, const vec3& direction)
    : origin(origin), direction(direction) {}

  vec3 pointAt(float t) const {
    return origin + t * direction;
  }

  vec3 origin, direction;
};

struct Hit {
  float t;
  vec3 p;
  vec3 normal;
};

Hit NoHit{0, vec3(0, 0, 0), vec3(0, 0, 0)};

bool operator==(const Hit& a, const Hit& b) {
  return a.normal[0] == b.normal[0] && a.normal[1] == b.normal[1] &&
    a.normal[2] == b.normal[2] && a.p[0] == b.p[0] && a.p[1] == b.p[1] &&
    a.p[2] == b.p[2] && a.t == b.t;
}

bool operator!=(const Hit& a, const Hit& b) {
  return !(a == b);
}

class Hitable {
public:
  virtual Hit hit(const Ray& r, float minT, float maxT) const = 0;
};

#endif
