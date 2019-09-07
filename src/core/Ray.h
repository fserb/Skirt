#ifndef __CORE_RAY_H__
#define __CORE_RAY_H__

#include "core/skirt.h"

namespace skirt {

class Ray {
 public:
  Ray() {}
  Ray(const Vector3& origin, const Vector3& direction)
      : origin(origin), direction(direction) {}
  bool HasNaNs() {
    return origin.HasNaNs() || direction.HasNaNs();
  }

  Vector3 origin;
  Vector3 direction;
}

std::ostream&
operator<<(std::ostream& os, const Ray& r) {
  os << "Ray[o=" << r.origin << ", d=" << r.direction << "]";
  return os;
}

}  // namespace skirt

#endif  // __CORE_RAY_H__
