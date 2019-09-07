#ifndef __CORE_SCENE_H__
#define __CORE_SCENE_H__

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

#endif  // __CORE_SCENE_H__
