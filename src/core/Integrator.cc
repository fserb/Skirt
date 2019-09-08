#include "core/Integrator.h"

#include "core/skirt.h"

namespace skirt {

FilmTile SamplerIntegrator::Render(int x, int y, int width, int height) {
  FilmTile tile(x, y, width, height);

  return tile;
}

}  // namespace skirt
