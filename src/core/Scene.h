#pragma once

#include "core/skirt.h"

#include "core/Element.h"
#include "core/Film.h"
#include "core/Integrator.h"

#include "loader/Loader.h"

namespace skirt {

struct Description {
  Vector3 lookAtFrom;
  Vector3 lookAtTo;

  string cameraType;
  float cameraFOV;
  float cameraAperture;
  float cameraFocusDistance;

  string integratorType;

  string filmType;
  string filmFilename;
  int width;
  int height;
};

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

  shared_ptr<Element> root;

  unique_ptr<Description> desc;

 private:
  DISALLOW_COPY_AND_ASSIGN(Scene);
};

}  // namespace skirt
