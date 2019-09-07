#ifndef __CORE_SKIRT_H__
#define __CORE_SKIRT_H__

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

#include <glog/logging.h>

#define INLINE __attribute__((always_inline)) inline
#define LIKELY(expr) (__builtin_expect(!!(expr), 1))
#define UNLIKELY(expr) (__builtin_expect(!!(expr), 0))

#define UNUSED __attribute__((unused))

using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::weak_ptr;

// These printf-like functions are implemented in terms of vsnprintf, so
// they use the same attribute for compile-time format string checking.

// Returns a string corresponding to printf-like formatting of the
// arguments.
std::string StringPrintf(const char* fmt, ...)
    __attribute__((__format__(__printf__, 1, 2)));

// Appends a printf-like formatting of the arguments to 'dst'.
void StringAppendF(std::string* dst, const char* fmt, ...)
    __attribute__((__format__(__printf__, 2, 3)));

// Appends a printf-like formatting of the arguments to 'dst'.
void StringAppendV(std::string* dst, const char* format, va_list ap)
    __attribute__((__format__(__printf__, 2, 0)));

#include "core/Vector3.h"
#include "core/math.h"

#endif
