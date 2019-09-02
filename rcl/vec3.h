#ifndef __VEC3_H__
#define __VEC3_H__

#include <cmath>

class vec3 {
 public:
  vec3() : vec3(0, 0, 0) {}
  vec3(float x, float y, float z) : x(e[0]), y(e[1]), z(e[2]) {
    e[0] = x;
    e[1] = y;
    e[2] = z;
  }

  inline const vec3& operator+() const { return *this; }
  inline vec3 operator-() const { return vec3(-x, -y, -z); }
  inline float operator[](int i) const { return e[i]; }
  inline float& operator[](int i) { return e[i]; }

  inline float sqlen() const {
    return x*x + y*y + z*z;
  }

  inline float length() const {
    return sqrt(x*x + y*y + z*z);
  }

  inline void normalize() {
    float k = 1 / length();
    x *= k; y *= k; z *= k;
  }

  inline vec3& operator+=(const vec3& o) {
    e[0] += o.e[0]; e[1] += o.e[1]; e[2] += o.e[2]; return *this;
  }
  inline vec3& operator-=(const vec3& o) {
    e[0] -= o.e[0]; e[1] -= o.e[1]; e[2] -= o.e[2]; return *this;
  }
  inline vec3& operator*=(const vec3& o) {
    e[0] *= o.e[0]; e[1] *= o.e[1]; e[2] *= o.e[2]; return *this;
  }
  inline vec3& operator/=(const vec3& o) {
    e[0] /= o.e[0]; e[1] /= o.e[1]; e[2] /= o.e[2]; return *this;
  }
  inline vec3& operator+=(const float t) {
    e[0] += t; e[1] += t; e[2] += t; return *this;
  }
  inline vec3& operator-=(const float t) {
    e[0] -= t; e[1] -= t; e[2] -= t; return *this;
  }
  inline vec3& operator*=(const float t) {
    e[0] *= t; e[1] *= t; e[2] *= t; return *this;
  }
  inline vec3& operator/=(const float t) {
    e[0] /= t; e[1] /= t; e[2] /= t; return *this;
  }

  float& x, y, z;
  float e[3];
};

inline vec3 operator+(const vec3& a, const vec3& b) {
  return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline vec3 operator-(const vec3& a, const vec3& b) {
  return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline vec3 operator/(const vec3& a, const vec3& b) {
  return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}
inline vec3 operator*(const vec3& a, const vec3& b) {
  return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}
inline vec3 operator+(const vec3& a, const float t) {
  return vec3(a.x + t, a.y + t, a.z + t);
}
inline vec3 operator-(const vec3& a, const float t) {
  return vec3(a.x - t, a.y - t, a.z - t);
}
inline vec3 operator*(const vec3& a, const float t) {
  return vec3(a.x * t, a.y * t, a.z * t);
}
inline vec3 operator/(const vec3& a, const float t) {
  return vec3(a.x / t, a.y / t, a.z / t);
}

inline float dot(const vec3& a, const vec3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(const vec3& a, const vec3& b) {
  return vec3(a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x);
}

inline vec3 unit(const vec3& v) {
  return v / v.length();
}

#endif // __VEC3_H__
