#include "core/Scene.h"

namespace skirt {

const Scene* Scene::Bake(unique_ptr<Scene>&& scene) {
  return scene.release();
}

Film Scene::MakeFilm() const {
  Film f(100, 100, "test.png");
  return f;
}

unique_ptr<Integrator> Scene::MakeIntegrator() const {
  unique_ptr<SamplerIntegrator> in(new SamplerIntegrator());

  return move(in);
}

}  // namespace skirt
