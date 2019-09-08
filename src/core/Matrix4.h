#pragma once

#include <cmath>

#include "core/AABB.h"
#include "core/Ray.h"
#include "core/Vector3.h"

namespace skirt {

class Matrix4 {
 public:
  Matrix4() : Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) {}

  Matrix4(float m00, float m01, float m02, float m03, float m10, float m11,
          float m12, float m13, float m20, float m21, float m22, float m23,
          float m30, float m31, float m32, float m33)
      : d{m00,
          m01,
          m02,
          m03,
          m10,
          m11,
          m12,
          m13,
          m20,
          m21,
          m22,
          m23,
          m30,
          m31,
          m32,
          m33} {
    DCHECK(!HasNaNs());
  }

  bool HasNaNs() {
    for (int i = 0; i < 16; ++i)
      if (std::isnan(d[i])) return true;
    return false;
  }

  INLINE Matrix4& CopyFrom(const Matrix4& m) {
    for (int i = 0; i < 16; ++i) d[i] = m[i];
    return *this;
  }

  INLINE float operator[](int i) const {
    return d[i];
  }
  INLINE float& operator[](int i) {
    return d[i];
  }

  INLINE Matrix4& Scale(const Vector3& v) {
    return Scale(v.x, v.y, v.z);
  }

  INLINE Matrix4& Scale(float v) {
    return Scale(v, v, v);
  }

  Matrix4& Scale(float x, float y, float z);

  INLINE Matrix4& Translate(const Vector3& v) {
    return Translate(v.x, v.y, v.z);
  }

  Matrix4& Translate(float x, float y, float z);

  Matrix4& Rotate(const Vector3& axis, float rad);

  Matrix4& RotateX(float rad);

  Matrix4& RotateY(float rad);
  Matrix4& RotateZ(float rad);

  Matrix4& RotateXYZ(float x, float y, float z);

  INLINE bool SwapsHandedness() const {
    float det = d[0] * (d[5] * d[10] - d[6] * d[9]) -
                d[1] * (d[4] * d[10] - d[6] * d[8]) +
                d[2] * (d[4] * d[9] - d[5] * d[8]);
    return det < 0;
  }

  float d[16];
};

Matrix4 Inverse(const Matrix4& mat);

Matrix4 LookAt(const Vector3& from, const Vector3& to, const Vector3& up);

INLINE Vector3 Transform(const Matrix4& mat, const Vector3& v) {
  float w = (mat[3] * v.x + mat[7] * v.y + mat[11] * v.z + mat[15]) || 1.0;
  return Vector3((mat[0] * v.x + mat[4] * v.y + mat[8] * v.z + mat[12]) / w,
                 (mat[1] * v.x + mat[5] * v.y + mat[9] * v.z + mat[13]) / w,
                 (mat[2] * v.x + mat[6] * v.y + mat[10] * v.z + mat[14]) / w);
}

INLINE Vector3 TransformNormal(const Matrix4& mat, const Vector3& v) {
  return Vector3(mat[0] * v.x + mat[4] * v.y + mat[8] * v.z,
                 mat[1] * v.x + mat[5] * v.y + mat[9] * v.z,
                 mat[2] * v.x + mat[6] * v.y + mat[10] * v.z);
}

INLINE Ray Transform(const Matrix4& mat, const Ray& ray) {
  Vector3 origin = Transform(mat, ray.origin);
  Vector3 direction = Transform(mat, ray.direction);

  return Ray(origin, direction, ray.minT, ray.maxT);
}

INLINE AABB Transform(const Matrix4& mat, const AABB& aabb) {
  AABB ret;

  for (int i = 0; i < 8; ++i) {
    Vector3 p = Transform(mat, aabb.Corner(i));
    ret = Union(ret, p);
  }

  return ret;
}

}  // namespace skirt
