#ifndef __VEC3_H__
#define __VEC3_H__

#include <cmath>
#include "skirt.h"

class vec3 {
 public:
  vec3() : vec3(0, 0, 0) {}
  vec3(float x, float y, float z) : x(x), y(y), z(z) { DCHECK(!HasNaNs()); }
  vec3(const vec3 &v) : vec3(v.x, v.y, v.z) {}

  bool HasNaNs() const {
    return std::isnan(x) || std::isnan(y) || std::isnan(z);
  }

  float operator[](int i) const {
    DCHECK(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }
  float &operator[](int i) {
    DCHECK(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  vec3 &operator=(const vec3 &v) {
    DCHECK(!v.HasNaNs());
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  vec3 operator+(const vec3 &v) const {
    DCHECK(!v.HasNaNs());
    return vec3(x + v.x, y + v.y, z + v.z);
  }

  vec3 &operator+=(const vec3 &v) {
    DCHECK(!v.HasNaNs());
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  float x, y, z;
};

#endif
