#pragma once

#include "core/skirt.h"

#include "core/AABB.h"
#include "core/Hit.h"
#include "core/Ray.h"

namespace skirt {

class Shape {
 public:
  virtual ~Shape() = default;
  virtual const AABB Bound() const = 0;
  virtual optional<Hit> Intersect(const Ray& r) const = 0;

  virtual float Area() const = 0;
};

}  // namespace skirt
