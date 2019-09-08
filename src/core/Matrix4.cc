
#include "core/Matrix4.h"

#include <cmath>

#include "core/Vector3.h"

namespace skirt {

Matrix4& Matrix4::Scale(float x, float y, float z) {
  d[0] *= x;
  d[1] *= x;
  d[2] *= x;
  d[3] *= x;
  d[4] *= y;
  d[5] *= y;
  d[6] *= y;
  d[7] *= y;
  d[8] *= z;
  d[9] *= z;
  d[10] *= z;
  d[11] *= z;
  return *this;
}

Matrix4& Matrix4::Translate(float x, float y, float z) {
  d[12] = d[0] * x + d[4] * y + d[8] * z + d[12];
  d[13] = d[1] * x + d[5] * y + d[9] * z + d[13];
  d[14] = d[2] * x + d[6] * y + d[10] * z + d[14];
  d[15] = d[3] * x + d[7] * y + d[11] * z + d[15];
  return *this;
}

Matrix4& Matrix4::Rotate(const Vector3& axis, float rad) {
  Vector3 v = Normalize(axis);
  float s = sin(rad);
  float c = cos(rad);
  float t = 1 - c;

  float a00 = d[0], a01 = d[1], a02 = d[2], a03 = d[3], a10 = d[4], a11 = d[5],
        a12 = d[6], a13 = d[7], a20 = d[8], a21 = d[9], a22 = d[10],
        a23 = d[11];

  float b00 = v.x * v.x * t + c, b01 = v.y * v.x * t + v.z * s,
        b02 = v.z * v.x * t - v.y * s, b10 = v.x * v.y * t - v.z * s,
        b11 = v.y * v.y * t + c, b12 = v.z * v.y * t + v.x * s,
        b20 = v.x * v.z * t + v.y * s, b21 = v.y * v.z * t - v.x * s,
        b22 = v.z * v.z * t + c;

  d[0] = a00 * b00 + a10 * b01 + a20 * b02;
  d[1] = a01 * b00 + a11 * b01 + a21 * b02;
  d[2] = a02 * b00 + a12 * b01 + a22 * b02;
  d[3] = a03 * b00 + a13 * b01 + a23 * b02;
  d[4] = a00 * b10 + a10 * b11 + a20 * b12;
  d[5] = a01 * b10 + a11 * b11 + a21 * b12;
  d[6] = a02 * b10 + a12 * b11 + a22 * b12;
  d[7] = a03 * b10 + a13 * b11 + a23 * b12;
  d[8] = a00 * b20 + a10 * b21 + a20 * b22;
  d[9] = a01 * b20 + a11 * b21 + a21 * b22;
  d[10] = a02 * b20 + a12 * b21 + a22 * b22;
  d[11] = a03 * b20 + a13 * b21 + a23 * b22;

  return *this;
}

Matrix4& Matrix4::RotateX(float rad) {
  float s = sin(rad), c = cos(rad), a10 = d[4], a11 = d[5], a12 = d[6],
        a13 = d[7], a20 = d[8], a21 = d[9], a22 = d[10], a23 = d[11];
  d[4] = a10 * c + a20 * s;
  d[5] = a11 * c + a21 * s;
  d[6] = a12 * c + a22 * s;
  d[7] = a13 * c + a23 * s;
  d[8] = a20 * c - a10 * s;
  d[9] = a21 * c - a11 * s;
  d[10] = a22 * c - a12 * s;
  d[11] = a23 * c - a13 * s;
  return *this;
}

Matrix4& Matrix4::RotateY(float rad) {
  float s = sin(rad), c = cos(rad), a00 = d[0], a01 = d[1], a02 = d[2],
        a03 = d[3], a20 = d[8], a21 = d[9], a22 = d[10], a23 = d[11];
  d[0] = a00 * c - a20 * s;
  d[1] = a01 * c - a21 * s;
  d[2] = a02 * c - a22 * s;
  d[3] = a03 * c - a23 * s;
  d[8] = a00 * s + a20 * c;
  d[9] = a01 * s + a21 * c;
  d[10] = a02 * s + a22 * c;
  d[11] = a03 * s + a23 * c;
  return *this;
}

Matrix4& Matrix4::RotateZ(float rad) {
  float s = sin(rad), c = cos(rad), a00 = d[0], a01 = d[1], a02 = d[2],
        a03 = d[3], a10 = d[4], a11 = d[5], a12 = d[6], a13 = d[7];

  d[0] = a00 * c + a10 * s;
  d[1] = a01 * c + a11 * s;
  d[2] = a02 * c + a12 * s;
  d[3] = a03 * c + a13 * s;
  d[4] = a10 * c - a00 * s;
  d[5] = a11 * c - a01 * s;
  d[6] = a12 * c - a02 * s;
  d[7] = a13 * c - a03 * s;
  return *this;
}

Matrix4& Matrix4::RotateXYZ(float x, float y, float z) {
  RotateX(x);
  RotateY(y);
  RotateZ(z);
  return *this;
}

Matrix4 Inverse(const Matrix4& mat) {
  float t11 = mat[9] * mat[14] * mat[7] - mat[13] * mat[10] * mat[7] +
              mat[13] * mat[6] * mat[11] - mat[5] * mat[14] * mat[11] -
              mat[9] * mat[6] * mat[15] + mat[5] * mat[10] * mat[15],
        t12 = mat[12] * mat[10] * mat[7] - mat[8] * mat[14] * mat[7] -
              mat[12] * mat[6] * mat[11] + mat[4] * mat[14] * mat[11] +
              mat[8] * mat[6] * mat[15] - mat[4] * mat[10] * mat[15],
        t13 = mat[8] * mat[13] * mat[7] - mat[12] * mat[9] * mat[7] +
              mat[12] * mat[5] * mat[11] - mat[4] * mat[13] * mat[11] -
              mat[8] * mat[5] * mat[15] + mat[4] * mat[9] * mat[15],
        t14 = mat[12] * mat[9] * mat[6] - mat[8] * mat[13] * mat[6] -
              mat[12] * mat[5] * mat[10] + mat[4] * mat[13] * mat[10] +
              mat[8] * mat[5] * mat[14] - mat[4] * mat[9] * mat[14];

  float det = mat[0] * t11 + mat[1] * t12 + mat[2] * t13 + mat[3] * t14;

  Matrix4 ret;
  CHECK_NE(det, 0);
  float detInv = 1 / det;
  ret[0] = t11 * detInv;
  ret[1] = (mat[13] * mat[10] * mat[3] - mat[9] * mat[14] * mat[3] -
            mat[13] * mat[2] * mat[11] + mat[1] * mat[14] * mat[11] +
            mat[9] * mat[2] * mat[15] - mat[1] * mat[10] * mat[15]) *
           detInv;
  ret[2] = (mat[5] * mat[14] * mat[3] - mat[13] * mat[6] * mat[3] +
            mat[13] * mat[2] * mat[7] - mat[1] * mat[14] * mat[7] -
            mat[5] * mat[2] * mat[15] + mat[1] * mat[6] * mat[15]) *
           detInv;
  ret[3] = (mat[9] * mat[6] * mat[3] - mat[5] * mat[10] * mat[3] -
            mat[9] * mat[2] * mat[7] + mat[1] * mat[10] * mat[7] +
            mat[5] * mat[2] * mat[11] - mat[1] * mat[6] * mat[11]) *
           detInv;

  ret[4] = t12 * detInv;
  ret[5] = (mat[8] * mat[14] * mat[3] - mat[12] * mat[10] * mat[3] +
            mat[12] * mat[2] * mat[11] - mat[0] * mat[14] * mat[11] -
            mat[8] * mat[2] * mat[15] + mat[0] * mat[10] * mat[15]) *
           detInv;
  ret[6] = (mat[12] * mat[6] * mat[3] - mat[4] * mat[14] * mat[3] -
            mat[12] * mat[2] * mat[7] + mat[0] * mat[14] * mat[7] +
            mat[4] * mat[2] * mat[15] - mat[0] * mat[6] * mat[15]) *
           detInv;
  ret[7] = (mat[4] * mat[10] * mat[3] - mat[8] * mat[6] * mat[3] +
            mat[8] * mat[2] * mat[7] - mat[0] * mat[10] * mat[7] -
            mat[4] * mat[2] * mat[11] + mat[0] * mat[6] * mat[11]) *
           detInv;

  ret[8] = t13 * detInv;
  ret[9] = (mat[12] * mat[9] * mat[3] - mat[8] * mat[13] * mat[3] -
            mat[12] * mat[1] * mat[11] + mat[0] * mat[13] * mat[11] +
            mat[8] * mat[1] * mat[15] - mat[0] * mat[9] * mat[15]) *
           detInv;
  ret[10] = (mat[4] * mat[13] * mat[3] - mat[12] * mat[5] * mat[3] +
             mat[12] * mat[1] * mat[7] - mat[0] * mat[13] * mat[7] -
             mat[4] * mat[1] * mat[15] + mat[0] * mat[5] * mat[15]) *
            detInv;
  ret[11] = (mat[8] * mat[5] * mat[3] - mat[4] * mat[9] * mat[3] -
             mat[8] * mat[1] * mat[7] + mat[0] * mat[9] * mat[7] +
             mat[4] * mat[1] * mat[11] - mat[0] * mat[5] * mat[11]) *
            detInv;

  ret[12] = t14 * detInv;
  ret[13] = (mat[8] * mat[13] * mat[2] - mat[12] * mat[9] * mat[2] +
             mat[12] * mat[1] * mat[10] - mat[0] * mat[13] * mat[10] -
             mat[8] * mat[1] * mat[14] + mat[0] * mat[9] * mat[14]) *
            detInv;
  ret[14] = (mat[12] * mat[5] * mat[2] - mat[4] * mat[13] * mat[2] -
             mat[12] * mat[1] * mat[6] + mat[0] * mat[13] * mat[6] +
             mat[4] * mat[1] * mat[14] - mat[0] * mat[5] * mat[14]) *
            detInv;
  ret[15] = (mat[4] * mat[9] * mat[2] - mat[8] * mat[5] * mat[2] +
             mat[8] * mat[1] * mat[6] - mat[0] * mat[9] * mat[6] -
             mat[4] * mat[1] * mat[10] + mat[0] * mat[5] * mat[10]) *
            detInv;

  return ret;
}

Matrix4 LookAt(const Vector3& from, const Vector3& to, const Vector3& up) {
  Vector3 dir = Normalize(to - from);
  Vector3 right = Normalize(Cross(Normalize(up), dir));
  // up has to be orthonomral to from and to.
  CHECK(dir.Length() == 0);
  Vector3 newUp = Cross(dir, right);

  // clang-format off
  Matrix4 mat(right.x, newUp.x, dir.x, from.x,
              right.y, newUp.y, dir.y, from.y,
              right.z, newUp.z, dir.z, from.z,
              0, 0, 0, 1);
  // clang-format on

  return Inverse(mat);
}

}  // namespace skirt
