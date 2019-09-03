#ifndef __RAY_H__
#define __RAY_H__

#include <emscripten.h>

#include "vec3.h"

using namespace std;

class Material;

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

class Camera {
public:
  Camera() {
    origin = vec3(0, 0, 0);
    lowerLeftCorner = vec3(-2, -1, -1);
    horizontal = vec3(4, 0 ,0);
    vertical = vec3(0, 2, 0);
  }

  Ray getRay(float u, float v) {
    return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
  }

  vec3 origin;
  vec3 lowerLeftCorner;
  vec3 horizontal;
  vec3 vertical;
};

struct Hit {
  float t;
  vec3 p;
  vec3 normal;
  shared_ptr<Material> material;

  operator bool() const {
    return normal[0] != 0 || normal[1] != 0 || normal[2] != 0;
  }
};

Hit NoHit{0, vec3(0, 0, 0), vec3(0, 0, 0)};


// bool operator==(const Hit& a, const Hit& b) {
//   return a.normal[0] == b.normal[0] && a.normal[1] == b.normal[1] &&
//     a.normal[2] == b.normal[2] && a.p[0] == b.p[0] && a.p[1] == b.p[1] &&
//     a.p[2] == b.p[2] && a.t == b.t;
// }
// bool operator!=(const Hit& a, const Hit& b) {
//   return !(a == b);
// }

class Hitable {
public:
  virtual Hit hit(const Ray& r, float minT, float maxT) const = 0;
};

#endif
