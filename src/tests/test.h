#pragma once

#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include "core/Vector3.h"

using namespace skirt;
using namespace rc;

template <>
struct Arbitrary<Vector3> {
  static Gen<Vector3> arbitrary() {
    return gen::build<Vector3>(
        gen::set(&Vector3::x), gen::set(&Vector3::y), gen::set(&Vector3::z));
  }
};

namespace skirt {

bool AlmostEqual(float A, float B, int maxUlpsDiff = 5) {
  int& a = reinterpret_cast<int&>(A);
  int& b = reinterpret_cast<int&>(B);

  // different signs means different, except for +0 and -0
  if ((a < 0) != (b < 0)) return a == b;

  int ulpsdiff = abs(a - b);
  return ulpsdiff <= maxUlpsDiff;
}

}  // namespace skirt
