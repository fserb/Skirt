#ifndef __CORE_MATH_H__
#define __CORE_MATH_H__

#include <algorithm>
#include <cmath>

using std::max;
using std::min;

template <typename T>
INLINE int sign(T a) {
  return a < 0 ? -1 : 1;
}

template <typename T>
INLINE T clamp(T v, T lo, T hi) {
  return max(lo, (min(v, hi)));
}

static constexpr float MaxFloat = std::numeric_limits<float>::max();
static constexpr float Infinity = std::numeric_limits<float>::infinity();
static constexpr float MachineEpsilon = std::numeric_limits<float>::epsilon();
static constexpr float ShadowEpsilon = 0.0001f;
static constexpr float Pi = 3.14159265358979323846;
static constexpr float Tau = 6.28318530717958647692;
static constexpr float Sqrt2 = 1.41421356237309504880;

#endif
