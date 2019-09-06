#ifndef __CORE_SKIRT_H__
#define __CORE_SKIRT_H__

#include <cstdlib>
#include <cmath>
#include <limits>
#include <iostream>

#include "glog/logging.h"

static constexpr float MaxFloat = std::numeric_limits<float>::max();
static constexpr float Infinity = std::numeric_limits<float>::infinity();
static constexpr float MachineEpsilon = std::numeric_limits<float>::epsilon();
static constexpr float ShadowEpsilon = 0.0001f;
static constexpr float Pi = 3.14159265358979323846;
static constexpr float Tau = 6.28318530717958647692;
static constexpr float Sqrt2 = 1.41421356237309504880;

#define LIKELY(x) (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect(!!(x), 0))

#endif
