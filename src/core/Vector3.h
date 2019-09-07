#ifndef __CORE_VECTOR3_H__
#define __CORE_VECTOR3_H__

#include <cmath>
#include "core/skirt.h"

namespace skirt {

class Vector3 {
 public:
  Vector3() : Vector3(0, 0, 0) {}
  Vector3(const Vector3& v) : Vector3(v.x, v.y, v.z) {}
  Vector3(float x, float y, float z) : x(x), y(y), z(z) { DCHECK(!HasNaNs()); }

  bool HasNaNs() const {
    return std::isnan(x) || std::isnan(y) || std::isnan(z);
  }

  INLINE float operator[](int i) const {
    DCHECK(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }
  INLINE float& operator[](int i) {
    DCHECK(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  INLINE Vector3& operator=(const Vector3& v) {
    DCHECK(!v.HasNaNs());
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  INLINE bool operator==(const Vector3& v) const {
    return x == v.x && y == v.y && z == v.z;
  }
  INLINE bool operator!=(const Vector3& v) const { return !(*this == v); }

  INLINE Vector3 operator-() const { return Vector3(-x, -y, -z); }

  INLINE float lensq() const { return x * x + y * y + z * z; }
  INLINE float length() const { return std::sqrt(lensq()); }

  INLINE void normalize() {
    float len = length();
    CHECK_NE(len, 0);
    float div = 1 / len;
    x *= div;
    y *= div;
    z *= div;
  }

  float x, y, z;
};

INLINE Vector3 operator+(const Vector3& a, const Vector3& b) {
  DCHECK(!a.HasNaNs() && !b.HasNaNs());
  return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

INLINE Vector3 operator-(const Vector3& a, const Vector3& b) {
  DCHECK(!a.HasNaNs() && !b.HasNaNs());
  return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

INLINE Vector3 operator*(const Vector3& a, float t) {
  DCHECK(!a.HasNaNs() && !std::isnan(t));
  return Vector3(a.x * t, a.y * t, a.z * t);
}

INLINE Vector3 operator/(const Vector3& a, float t) {
  DCHECK(!a.HasNaNs() && !std::isnan(t));
  CHECK_NE(t, 0);
  return a * (1.0f / t);
}

INLINE Vector3& operator+=(Vector3& a, const Vector3& b) { return a = a + b; }
INLINE Vector3& operator-=(Vector3& a, const Vector3& b) { return a = a - b; }
INLINE Vector3& operator*=(Vector3& a, float t) { return a = a * t; }
INLINE Vector3& operator/=(Vector3& a, float t) {
  CHECK_NE(t, 0);
  return a = a * (1.0f / t);
}

INLINE Vector3 operator*(float t, const Vector3& a) { return a * t; }

INLINE Vector3 operator/(float t, const Vector3& a) {
  CHECK_NE(t, 0);
  return a * (1.0f / t);
}

INLINE Vector3 min(const Vector3& a, const Vector3& b) {
  return Vector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

INLINE Vector3 max(const Vector3& a, const Vector3& b) {
  return Vector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

INLINE float dot(const Vector3& a, const Vector3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

INLINE Vector3 cross(const Vector3& a, const Vector3& b) {
  return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}

INLINE Vector3 unit(const Vector3& v) { return v / v.length(); }

INLINE Vector3 reflect(const Vector3& v, const Vector3& n) {
  return v - (2 * dot(v, n)) * n;
}

INLINE Vector3 refract(const Vector3& v, const Vector3& normal, float nint) {
  Vector3 uv = unit(v);
  float dt = dot(uv, normal);
  float discr = 1.0 - nint * nint * (1 - dt * dt);
  if (discr <= 0) return Vector3(0, 0, 0);
  return nint * (uv - normal * dt) - normal * sqrt(discr);
}

INLINE std::ostream& operator<<(std::ostream& os, const Vector3& v) {
  os << StringPrintf("[ %f, %f, %f ]", v.x, v.y, v.z);
  return os;
}

}  // namespace skirt

#endif  // __CORE_VECTOR3_H__
