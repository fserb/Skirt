#ifndef __CORE_AABB_H__
#define __CORE_AABB_H__

#include <cmath>

#include "core/Vector3.h"

using namespace std;

namespace skirt {

class AABB {
 public:
  AABB(const Vector3& min, const Vector3& max) : min(min), max(max) {}

  Vector3 min, max;
};

}  // namespace skirt

#endif  // __CORE_AABB_H__
