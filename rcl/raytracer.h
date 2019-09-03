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
  Camera() {}
  Camera(vec3 lookfrom, vec3 lookat, vec3 up, float vfov, float aspect, float aperture, float focusDist) {
    lensRadius = aperture / 2;
    float theta = vfov * M_PI / 180.;
    float halfHeight = tan(theta / 2);
    float halfWidth = aspect * halfHeight;

    origin = lookfrom;
    w = unit(lookfrom - lookat);
    u = unit(cross(up, w));
    v = cross(w, u);

    lowerLeftCorner = origin - halfWidth * u  * focusDist
      - halfHeight * v  * focusDist - w * focusDist;
    horizontal = 2 * halfWidth * u * focusDist;
    vertical = 2 * halfHeight * v * focusDist;
  }

  Ray getRay(float s, float t) {
    vec3 rd = lensRadius * randomInUnitDisk();
    vec3 offset = u * rd.x() + v * rd.y();
    return Ray(origin + offset,
      lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
  }

  vec3 origin;
  vec3 lowerLeftCorner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  float lensRadius;
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
