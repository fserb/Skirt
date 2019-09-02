#ifndef __VEC3_H__
#define __VEC3_H__

#include <cmath>

class vec3 {
 public:
  vec3() : vec3(0, 0, 0) {}
  vec3(float x, float y, float z) {
    e[0] = x;
    e[1] = y;
    e[2] = z;
  }

  inline float x() const { return e[0]; }
  inline float y() const { return e[1]; }
  inline float z() const { return e[2]; }

  inline const vec3& operator+() const { return *this; }
  inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  inline float operator[](int i) const { return e[i]; }
  inline float& operator[](int i) { return e[i]; }

  inline float sqlen() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  inline float length() const {
    return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
  }

  inline void normalize() {
    float k = 1 / length();
    e[0] *= k; e[1] *= k; e[2] *= k;
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

  float e[3];
};

inline vec3 operator+(const vec3& a, const vec3& b) {
  return vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
inline vec3 operator-(const vec3& a, const vec3& b) {
  return vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
inline vec3 operator/(const vec3& a, const vec3& b) {
  return vec3(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
}
inline vec3 operator*(const vec3& a, const vec3& b) {
  return vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}
inline vec3 operator+(const vec3& a, const float t) {
  return vec3(a[0] + t, a[1] + t, a[2] + t);
}
inline vec3 operator-(const vec3& a, const float t) {
  return vec3(a[0] - t, a[1] - t, a[2] - t);
}
inline vec3 operator*(const vec3& a, const float t) {
  return vec3(a[0] * t, a[1] * t, a[2] * t);
}
inline vec3 operator/(const vec3& a, const float t) {
  return vec3(a[0] / t, a[1] / t, a[2] / t);
}
inline vec3 operator+(const float t, const vec3& a) {
  return vec3(a[0] + t, a[1] + t, a[2] + t);
}
inline vec3 operator-(const float t, const vec3& a) {
  return vec3(a[0] - t, a[1] - t, a[2] - t);
}
inline vec3 operator*(const float t, const vec3& a) {
  return vec3(a[0] * t, a[1] * t, a[2] * t);
}
inline vec3 operator/(const float t, const vec3& a) {
  return vec3(a[0] / t, a[1] / t, a[2] / t);
}

inline float dot(const vec3& a, const vec3& b) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline vec3 cross(const vec3& a, const vec3& b) {
  return vec3(a[1] * b[2] - a[2] * b[1],
    a[2] * b[0] - a[0] * b[2],
    a[0] * b[1] - a[1] * b[0]);
}

inline vec3 unit(const vec3& v) {
  return v / v.length();
}

inline std::ostream& operator<<(std::ostream& os, const vec3& t) {
  os << "vec3(" << t[0] << ", " << t[1] << ", " << t[2] << ")";
  return os;
}

#endif // __VEC3_H__
