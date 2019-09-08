#pragma once

#include "core/skirt.h"

namespace skirt {

class Scene {
 public:
  Scene() {}

  const Scene* Bake(unique_ptr<Scene>&& scene);

 private:
  // shared_ptr<Element> root;
};

}  // namespace skirt
