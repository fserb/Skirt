#include "core/Integrator.h"

#include "core/Hit.h"
#include "core/Scene.h"
#include "core/skirt.h"

namespace skirt {

Vector3 SamplerIntegrator::color(const Ray& r) {
  optional<Hit> hit = scene->root->Intersect(r);
  if (hit) {
    Vector3 n = Normalize(hit->p - Vector3(0, 0, -1));
    return 0.5 * (n + Vector3(1, 1, 1));
  }

  Vector3 ud = Normalize(r.direction);
  float t = 0.5 * (ud.y + 1);
  return (1.0 - t) * Vector3(1, 1, 1) + t * Vector3(0.5, 0.7, 1.0);
}

FilmTile SamplerIntegrator::Render(int x0, int y0, int width, int height) {
  FilmTile tile(x0, y0, width, height);

  int WIDTH = 200;
  int HEIGHT = 100;

  Vector3 llc(-2, -1, -1);
  Vector3 hor(4, 0, 0);
  Vector3 ver(0, 2, 0);
  Vector3 origin(0, 0, 0);

  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      int x = x0 + i;
      int y = HEIGHT - y0 - height + j;

      float u = float(x) / WIDTH;
      float v = float(y) / HEIGHT;

      Ray r(origin, llc + u * hor + v * ver);

      // color.
      Vector3 c = color(r);

      tile.WritePixel(i, j, c);
    }
  }

  return tile;
}

}  // namespace skirt
