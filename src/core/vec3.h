#ifndef __VEC3_H__
#define __VEC3_H__

#include <cmath>
#include "core/skirt.h"

namespace skirt {

class vec3 {
 public:
  vec3() : vec3(0, 0, 0) {}
  vec3(const vec3& v) : vec3(v.x, v.y, v.z) {}
  vec3(float x, float y, float z) : x(x), y(y), z(z) { DCHECK(!HasNaNs()); }

  bool HasNaNs() const {
    return std::isnan(x) || std::isnan(y) || std::isnan(z);
  }

  float operator[](int i) const {
    DCHECK(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }
  float& operator[](int i) {
    DCHECK(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  vec3& operator=(const vec3& v) {
    DCHECK(!v.HasNaNs());
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  bool operator==(const vec3& v) const {
    return x == v.x && y == v.y && z == v.z;
  }
  bool operator!=(const vec3& v) const { return !(*this == v); }

  vec3 operator-() const { return vec3(-x, -y, -z); }

  float lensq() const { return x * x + y * y + z * z; }
  float length() const { return std::sqrt(lensq()); }

  void normalize() {
    float len = length();
    CHECK_NE(len, 0);
    float div = 1 / len;
    x *= div;
    y *= div;
    z *= div;
  }

  float x, y, z;
};

vec3 operator+(const vec3& a, const vec3& b) {
  DCHECK(!a.HasNaNs() && !b.HasNaNs());
  return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator-(const vec3& a, const vec3& b) {
  DCHECK(!a.HasNaNs() && !b.HasNaNs());
  return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 operator*(const vec3& a, float t) {
  DCHECK(!a.HasNaNs() && !std::isnan(t));
  return vec3(a.x * t, a.y * t, a.z * t);
}

vec3 operator/(const vec3& a, float t) {
  DCHECK(!a.HasNaNs() && !std::isnan(t));
  CHECK_NE(t, 0);
  return a * (1.0f / t);
}

vec3& operator+=(vec3& a, const vec3& b) { return a = a + b; }
vec3& operator-=(vec3& a, const vec3& b) { return a = a - b; }
vec3& operator*=(vec3& a, float t) { return a = a * t; }
vec3& operator/=(vec3& a, float t) {
  CHECK_NE(t, 0);
  return a = a * (1.0f / t);
}

vec3 operator*(float t, const vec3& a) { return a * t; }

vec3 operator/(float t, const vec3& a) {
  CHECK_NE(t, 0);
  return a * (1.0f / t);
}

vec3 min(const vec3& a, const vec3& b) {
  return vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

vec3 max(const vec3& a, const vec3& b) {
  return vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

float dot(const vec3& a, const vec3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross(const vec3& a, const vec3& b) {
  return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

vec3 unit(const vec3& v) { return v / v.length(); }

vec3 reflect(const vec3& v, const vec3& n) { return v - (2 * dot(v, n)) * n; }

vec3 refract(const vec3& v, const vec3& normal, float nint) {
  vec3 uv = unit(v);
  float dt = dot(uv, normal);
  float discr = 1.0 - nint * nint * (1 - dt * dt);
  if (discr <= 0) return vec3(0, 0, 0);
  return nint * (uv - normal * dt) - normal * sqrt(discr);
}

std::ostream& operator<<(std::ostream& os, const vec3& v) {
  os << StringPrintf("[ %f, %f, %f ]", v.x, v.y, v.z);
  return os;
}

}  // namespace skirt

#endif
