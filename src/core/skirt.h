#pragma once

#include "config.h"

#include <stdarg.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <string>

#include <glog/logging.h>

#define INLINE __attribute__((always_inline)) inline
#define LIKELY(expr) (__builtin_expect(!!(expr), 1))
#define UNLIKELY(expr) (__builtin_expect(!!(expr), 0))

#define UNUSED __attribute__((unused))

#define DISALLOW_COPY_AND_ASSIGN(TypeName)       \
  TypeName& operator=(const TypeName&) = delete; \
  TypeName(const TypeName&) = delete;

namespace skirt {

using std::make_tuple;
using std::move;
using std::nullopt;
using std::optional;
using std::shared_ptr;
using std::string;
using std::tuple;
using std::unique_ptr;
using std::weak_ptr;

}  // namespace skirt

#include "core/StringPrintf.h"
#include "core/Vector3.h"
#include "core/math.h"
