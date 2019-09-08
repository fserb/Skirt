#include "test.h"

#include "core/Matrix4.h"
#include "core/skirt.h"

RC_GTEST_PROP(Matrix4, TransformRotate,
              (Vector3 v, Vector3 axis, float angle)) {
  RC_PRE(axis.Length() != 0);
  Matrix4 m;
  m.Rotate(axis, angle);
  Vector3 o = Transform(m, v);

  RC_ASSERT(AlmostEqual(o.Length(), v.Length()));
}
