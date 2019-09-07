#ifndef __CORE_MAT4_H__
#define __CORE_MAT4_H__

#include <cmath>

#include "core/Vector3.h"

using namespace std;

namespace skirt {

class Mat4 {
 public:
  Mat4() : d{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1} {}

  Mat4& copyFrom(const Mat4& m) {
    for (int i = 0; i < 16; ++i) d[i] = m[i];
    return *this;
  }

  inline float operator[](int i) const { return d[i]; }
  inline float& operator[](int i) { return d[i]; }

  Mat4& scale(const Vector3& v) { return scale(v.x, v.y, v.z); }

  Mat4& scale(float v) { return scale(v, v, v); }

  Mat4& scale(float x, float y, float z) {
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

  Mat4& translate(const Vector3& v) { return translate(v.x, v.y, v.z); }

  Mat4& translate(float x, float y, float z) {
    d[12] = d[0] * x + d[4] * y + d[8] * z + d[12];
    d[13] = d[1] * x + d[5] * y + d[9] * z + d[13];
    d[14] = d[2] * x + d[6] * y + d[10] * z + d[14];
    d[15] = d[3] * x + d[7] * y + d[11] * z + d[15];
    return *this;
  }

  Mat4& rotate(const Vector3& axis, float rad) {
    Vector3 v = unit(axis);
    float s = sin(rad);
    float c = cos(rad);
    float t = 1 - c;

    float a00 = d[0], a01 = d[1], a02 = d[2], a03 = d[3], a10 = d[4],
          a11 = d[5], a12 = d[6], a13 = d[7], a20 = d[8], a21 = d[9],
          a22 = d[10], a23 = d[11];

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

  Mat4& rotateX(float rad) {
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

  Mat4& rotateY(float rad) {
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

  Mat4& rotateZ(float rad) {
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

  Mat4& rotateXYZ(float x, float y, float z) {
    rotateX(x);
    rotateY(y);
    rotateZ(z);
    return *this;
  }

  Mat4& invert() {
    float n11 = d[0], n21 = d[1], n31 = d[2], n41 = d[3], n12 = d[4],
          n22 = d[5], n32 = d[6], n42 = d[7], n13 = d[8], n23 = d[9],
          n33 = d[10], n43 = d[11], n14 = d[12], n24 = d[13], n34 = d[14],
          n44 = d[15],

          t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 -
                n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44,
          t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 +
                n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44,
          t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 -
                n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44,
          t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 +
                n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;

    if (det == 0) {
      assert(false);
      return *this;
    }

    float detInv = 1 / det;
    d[0] = t11 * detInv;
    d[1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 +
            n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) *
           detInv;
    d[2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 -
            n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) *
           detInv;
    d[3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 +
            n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) *
           detInv;

    d[4] = t12 * detInv;
    d[5] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 -
            n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) *
           detInv;
    d[6] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 +
            n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) *
           detInv;
    d[7] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 -
            n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) *
           detInv;

    d[8] = t13 * detInv;
    d[9] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 +
            n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) *
           detInv;
    d[10] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 -
             n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) *
            detInv;
    d[11] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 +
             n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) *
            detInv;

    d[12] = t14 * detInv;
    d[13] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 -
             n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) *
            detInv;
    d[14] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 +
             n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) *
            detInv;
    d[15] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 -
             n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) *
            detInv;

    return *this;
  }

  float d[16];
};

inline Vector3 transform(const Vector3& v, const Mat4& mat) {
  float w = (mat[3] * v.x + mat[7] * v.y + mat[11] * v.z + mat[15]) || 1.0;
  return Vector3((mat[0] * v.x + mat[4] * v.y + mat[8] * v.z + mat[12]) / w,
                 (mat[1] * v.x + mat[5] * v.y + mat[9] * v.z + mat[13]) / w,
                 (mat[2] * v.x + mat[6] * v.y + mat[10] * v.z + mat[14]) / w);
}

inline Vector3 transformNormal(const Vector3& v, const Mat4& mat) {
  return Vector3(mat[0] * v.x + mat[4] * v.y + mat[8] * v.z,
                 mat[1] * v.x + mat[5] * v.y + mat[9] * v.z,
                 mat[2] * v.x + mat[6] * v.y + mat[10] * v.z);
}

}  // namespace skirt

#endif  // __CORE_MAT4_H__
