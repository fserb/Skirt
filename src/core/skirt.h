#ifndef __CORE_SKIRT_H__
#define __CORE_SKIRT_H__

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

#include <glog/logging.h>

static constexpr float MaxFloat = std::numeric_limits<float>::max();
static constexpr float Infinity = std::numeric_limits<float>::infinity();
static constexpr float MachineEpsilon = std::numeric_limits<float>::epsilon();
static constexpr float ShadowEpsilon = 0.0001f;
static constexpr float Pi = 3.14159265358979323846;
static constexpr float Tau = 6.28318530717958647692;
static constexpr float Sqrt2 = 1.41421356237309504880;

#define LIKELY(x) (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect(!!(x), 0))

// These printf-like functions are implemented in terms of vsnprintf, so they
// use the same attribute for compile-time format string checking.

// Returns a string corresponding to printf-like formatting of the arguments.
std::string StringPrintf(const char* fmt, ...)
    __attribute__((__format__(__printf__, 1, 2)));

// Appends a printf-like formatting of the arguments to 'dst'.
void StringAppendF(std::string* dst, const char* fmt, ...)
    __attribute__((__format__(__printf__, 2, 3)));

// Appends a printf-like formatting of the arguments to 'dst'.
void StringAppendV(std::string* dst, const char* format, va_list ap)
    __attribute__((__format__(__printf__, 2, 0)));

#endif
