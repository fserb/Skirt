#pragma once

#include "core/Film.h"
#include "core/Ray.h"
#include "core/skirt.h"

namespace skirt {

class Scene;

class Integrator {
 public:
  Integrator(const Scene* scene) : scene(scene) {}
  virtual ~Integrator() = default;
  virtual FilmTile Render(int x, int y, int width, int height) = 0;

  const Scene* scene;
  DISALLOW_COPY_AND_ASSIGN(Integrator);
};

class SamplerIntegrator : public Integrator {
 public:
  SamplerIntegrator(const Scene* scene) : Integrator(scene) {}
  virtual FilmTile Render(int x, int y, int width, int height) final;

 private:
  Vector3 color(const Ray& r);

  DISALLOW_COPY_AND_ASSIGN(SamplerIntegrator);
};

}  // namespace skirt
