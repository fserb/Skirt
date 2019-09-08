#include "core/Element.h"

#include "core/skirt.h"

namespace skirt {

Element::~Element() {}

const AABB Element::Bound() const {
  return shape->Bound();
}

optional<Hit> Element::Intersect(const Ray& r) const {
  return shape->Intersect(r);
}

}  // namespace skirt
