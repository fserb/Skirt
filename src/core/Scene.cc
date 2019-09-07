#include "core/Scene.h"

namespace skirt {

const Scene* Scene::Bake(unique_ptr<Scene>&& scene) { return scene.release(); }

}  // namespace skirt
