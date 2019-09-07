#ifndef __CORE_SKIRT_H__
#define __CORE_SKIRT_H__

#include <stdarg.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

#include <glog/logging.h>

#define INLINE __attribute__((always_inline)) inline
#define LIKELY(expr) (__builtin_expect(!!(expr), 1))
#define UNLIKELY(expr) (__builtin_expect(!!(expr), 0))

#define UNUSED __attribute__((unused))

namespace skirt {

using std::make_tuple;
using std::move;
using std::shared_ptr;
using std::string;
using std::tuple;
using std::unique_ptr;
using std::weak_ptr;

}  // namespace skirt

#include "core/StringPrintf.h"
#include "core/Vector3.h"
#include "core/math.h"

#endif
