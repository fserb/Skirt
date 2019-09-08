#pragma once

#include "core/Film.h"
#include "core/skirt.h"

namespace skirt {

class Integrator {
 public:
  virtual ~Integrator() = default;
  virtual FilmTile Render(int x, int y, int width, int height) = 0;
};

class SamplerIntegrator : public Integrator {
 public:
  virtual FilmTile Render(int x, int y, int width, int height) final;
};

}  // namespace skirt
