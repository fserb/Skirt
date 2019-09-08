#pragma once

#include "core/Vector3.h"
#include "core/skirt.h"

namespace skirt {

class Hit {
 public:
  Vector3 p;
  Vector3 normal;
};

}  // namespace skirt
