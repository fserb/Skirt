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

namespace skirt {}  // namespace skirt
