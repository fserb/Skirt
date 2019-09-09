#pragma once

#include "core/Vector3.h"
#include "core/skirt.h"

class Element;

namespace skirt {

class Hit {
 public:
  Hit(float t, const Vector3& p, const Vector3& normal)
      : t(t), p(p), normal(normal) {}

  float t;
  Vector3 p;
  Vector3 normal;
  Element* element;
};

}  // namespace skirt
