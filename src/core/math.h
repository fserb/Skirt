#pragma once

#include <algorithm>
#include <cmath>

namespace skirt {

using std::max;
using std::min;

static constexpr float MaxFloat = std::numeric_limits<float>::max();
static constexpr float Infinity = std::numeric_limits<float>::infinity();
static constexpr float MachineEpsilon =
    std::numeric_limits<float>::epsilon() * 0.5f;
static constexpr float ShadowEpsilon = 0.0001f;
static constexpr float PI = 3.14159265358979323846;
static constexpr float TAU = 6.28318530717958647692;
static constexpr float SQRT2 = 1.41421356237309504880;

template <typename T>
INLINE int sign(T a) {
  return a < 0 ? -1 : 1;
}

template <typename T>
INLINE T clamp(T v, T lo, T hi) {
  return max(lo, (min(v, hi)));
}

INLINE int FloatToBits(float f) {
  unsigned int i;
  memcpy(&i, &f, sizeof(float));
  return i;
}

INLINE float BitsToFloat(unsigned int i) {
  float f;
  memcpy(&f, &i, sizeof(unsigned int));
  return f;
}

INLINE float FloatUp(float v) {
  if (std::isinf(v) && v > 0.) return v;
  if (v == -0.f) v = 0.f;
  unsigned int i = FloatToBits(v);
  if (v >= 0) {
    ++i;
  } else {
    --i;
  }
  return BitsToFloat(i);
}

INLINE float FloatDown(float v) {
  if (std::isinf(v) && v < 0.) return v;
  if (v == 0.f) v = -0.f;
  unsigned int i = FloatToBits(v);
  if (v > 0) {
    --i;
  } else {
    ++i;
  }
  return BitsToFloat(i);
}

INLINE constexpr float gamma(int n) {
  return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
}

}  // namespace skirt
