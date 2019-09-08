#include "test.h"

#include "core/Matrix4.h"
#include "core/skirt.h"

bool AlmostEqual(float A, float B, int maxUlpsDiff = 5) {
  int& a = reinterpret_cast<int&>(A);
  int& b = reinterpret_cast<int&>(B);

  // different signs means different, except for +0 and -0
  if ((a < 0) != (b < 0)) return a == b;

  int ulpsdiff = abs(a - b);
  return ulpsdiff <= maxUlpsDiff;
}

RC_GTEST_PROP(Matrix4, TransformRotate,
              (Vector3 v, Vector3 axis, float angle)) {
  RC_PRE(axis.Length() != 0);
  Matrix4 m;
  m.Rotate(axis, angle);
  Vector3 o = Transform(m, v);

  RC_ASSERT(AlmostEqual(o.Length(), v.Length()));
}
