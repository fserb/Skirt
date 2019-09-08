#pragma once

#include "core/skirt.h"

#include "core/Hit.h"
#include "core/Shape.h"

namespace skirt {

class Sphere : public Shape {
 public:
  Sphere(float radius) : radius(radius) {}

  virtual const AABB Bound() const;
  virtual optional<Hit> Intersect(const Ray& r) const;

  virtual float Area() const;

  float radius;
};

}  // namespace skirt
