#pragma once

#include "core/skirt.h"

#include "core/AABB.h"
#include "core/Hit.h"
#include "core/Ray.h"

namespace skirt {

class Element {
 public:
  virtual ~Element();
  virtual const AABB& Bound() const = 0;
  virtual optional<Hit> Hit(const Ray& r) const = 0;

 private:
};

}  // namespace skirt
