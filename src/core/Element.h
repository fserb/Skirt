#pragma once

#include "core/skirt.h"

#include "core/AABB.h"
#include "core/Hit.h"
#include "core/Ray.h"
#include "core/Shape.h"

namespace skirt {

class Element {
 public:
  Element(shared_ptr<Shape> shape) : shape(shape) {}
  virtual ~Element();
  virtual const AABB Bound() const;
  virtual optional<Hit> Intersect(const Ray& r) const;

 private:
  shared_ptr<Shape> shape;
};

}  // namespace skirt
