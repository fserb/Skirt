
#include "test.h"

#include "core/skirt.h"

namespace skirt {

RC_GTEST_PROP(math, FloatToBits, (float f)) {
  unsigned int i = FloatToBits(f);
  float f2 = BitsToFloat(i);
  RC_ASSERT(f == f2);
}

RC_GTEST_PROP(math, BitsToFloat, (unsigned int i)) {
  float f = BitsToFloat(i);
  unsigned int i2 = FloatToBits(f);
  RC_ASSERT(i == i2);
}

RC_GTEST_PROP(math, FloatUpDown, (float f)) {
  float f2 = FloatUp(f);
  float f3 = FloatDown(f2);
  RC_ASSERT(f3 == f);
}

RC_GTEST_PROP(math, FloatDownUp, (float f)) {
  float f2 = FloatDown(f);
  float f3 = FloatUp(f2);
  RC_ASSERT(f3 == f);
}

}  // namespace skirt
