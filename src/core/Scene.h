#pragma once

#include "core/skirt.h"

#include "core/Element.h"
#include "core/Film.h"
#include "core/Integrator.h"

namespace skirt {

/*
A scene is modifiable during construction.
Once Bake() is called. The whole scene is const.
*/
class Scene {
 public:
  Scene() {}

  Film MakeFilm() const;
  unique_ptr<Integrator> MakeIntegrator() const;

  const Scene* Bake(unique_ptr<Scene>&& scene);

  INLINE void SetRootElement(shared_ptr<Element> root) {
    this->root = root;
  }

 private:
  shared_ptr<Element> root;

  DISALLOW_COPY_AND_ASSIGN(Scene);
};

}  // namespace skirt
