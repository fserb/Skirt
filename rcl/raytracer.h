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

class AABB {
public:
  AABB() {}
  AABB(const vec3& a, const vec3& b) : min(a), max(b) {}

  bool hit(const Ray& r, float minT, float maxT) const {
    for (int a = 0; a < 3; ++a) {
      float invD = 1.0 / r.direction[a];
      float t0 = (min[a] - r.origin[a]) * invD;
      float t1 = (max[a] - r.origin[a]) * invD;
      if (invD < 0) swap(t0, t1);
      minT = t0 > minT ? t0 : minT;
      maxT = t1 < maxT ? t1 : maxT;
      if (maxT < minT) return false;
    }
    return true;
  }

  operator bool() const {
    return min[0] != max[0] || min[1] != max[1] || min[2] != max[2];
  }

  vec3 min, max;
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

class Hitable {
public:
  virtual Hit hit(const Ray& r, float minT, float maxT) const = 0;
  virtual const AABB bbox() const = 0;
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



#endif
