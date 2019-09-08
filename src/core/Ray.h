#pragma once

#include "core/skirt.h"

namespace skirt {

class Ray {
 public:
  Ray() {}
  Ray(const Vector3& origin, const Vector3& direction)
      : Ray(origin, direction, 0, Infinity) {}
  Ray(const Vector3& origin, const Vector3& direction, float minT, float maxT)
      : origin(origin), direction(direction), minT(minT), maxT(maxT) {}
  bool HasNaNs() {
    return origin.HasNaNs() || direction.HasNaNs();
  }

  INLINE Vector3 pointAt(float t) const {
    return origin + direction * t;
  }

  Vector3 origin;
  Vector3 direction;
  float minT;
  float maxT;
};

INLINE std::ostream& operator<<(std::ostream& os, const Ray& r) {
  os << "Ray[o=" << r.origin << ", d=" << r.direction << "]";
  return os;
}

}  // namespace skirt
