#pragma once

#include <cmath>
#include "core/skirt.h"

namespace skirt {

template <typename T>
class Vector2 {
 public:
  Vector2() : Vector2(0, 0, 0) {}
  Vector2(const Vector2<T>& v) : Vector2(v.x, v.y) {}
  Vector2(T x, T y) : x(x), y(y) {
    DCHECK(!HasNaNs());
  }

  bool HasNaNs() const {
    return std::isnan(x) || std::isnan(y);
  }

  INLINE T operator[](int i) const {
    DCHECK(i >= 0 && i <= 1);
    return i == 0 ? x : y;
  }
  INLINE T& operator[](int i) {
    DCHECK(i >= 0 && i <= 1);
    return i == 0 ? x : y;
  }

  INLINE Vector2<T>& operator=(const Vector2<T>& v) {
    DCHECK(!v.HasNaNs());
    x = v.x;
    y = v.y;
    return *this;
  }

  INLINE bool operator==(const Vector2<T>& v) const {
    return x == v.x && y == v.y;
  }
  INLINE bool operator!=(const Vector2<T>& v) const {
    return !(*this == v);
  }

  INLINE Vector2<T> operator-() const {
    return Vector2(-x, -y);
  }

  INLINE float LengthSq() const {
    return x * x + y * y;
  }
  INLINE float Length() const {
    return std::sqrt(LengthSq());
  }

  INLINE void Normalize() {
    float len = Length();
    CHECK_NE(len, 0);
    float div = 1 / len;
    x *= div;
    y *= div;
  }

  T x, y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

template <typename T>
INLINE Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b) {
  DCHECK(!a.HasNaNs() && !b.HasNaNs());
  return Vector2<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
INLINE Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b) {
  DCHECK(!a.HasNaNs() && !b.HasNaNs());
  return Vector2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
INLINE Vector2<T> operator*(const Vector2<T>& a, T t) {
  DCHECK(!a.HasNaNs() && !std::isnan(t));
  return Vector2<T>(a.x * t, a.y * t);
}

template <typename T>
INLINE Vector2<T> operator/(const Vector2<T>& a, T t) {
  DCHECK(!a.HasNaNs() && !std::isnan(t));
  CHECK_NE(t, 0);
  return a * (1.0f / t);
}

template <typename T>
INLINE Vector2<T>& operator+=(Vector2<T>& a, const Vector2<T>& b) {
  return a = a + b;
}
template <typename T>
INLINE Vector2<T>& operator-=(Vector2<T>& a, const Vector2<T>& b) {
  return a = a - b;
}
template <typename T>
INLINE Vector2<T>& operator*=(Vector2<T>& a, T t) {
  return a = a * t;
}
template <typename T>
INLINE Vector2<T>& operator/=(Vector2<T>& a, T t) {
  CHECK_NE(t, 0);
  return a = a * (1.0f / t);
}

template <typename T>
INLINE Vector2<T> operator*(T t, const Vector2<T>& a) {
  return a * t;
}

template <typename T>
INLINE Vector2<T> operator/(T t, const Vector2<T>& a) {
  CHECK_NE(t, 0);
  return a * (1.0f / t);
}

template <typename T>
INLINE Vector2<T> min(const Vector2<T>& a, const Vector2<T>& b) {
  return Vector2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
}

template <typename T>
INLINE Vector2<T> max(const Vector2<T>& a, const Vector2<T>& b) {
  return Vector2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
}

template <typename T>
INLINE float Distance(const Vector2<T>& a, const Vector2<T>& b) {
  return (b - a).Length();
}

template <typename T>
INLINE float Dot(const Vector2<T>& a, const Vector2<T>& b) {
  return a.x * b.x + a.y * b.y;
}

template <typename T>
INLINE Vector2<T> Normalize(const Vector2<T>& v) {
  return v / v.Length();
}

template <typename T>
INLINE std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
  os << "[ " << v.x << ", " << v.y << " ]";
  return os;
}

}  // namespace skirt
