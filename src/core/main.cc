// main

#include "core/skirt.h"

#include <stdio.h>

#include "loader/Loader.h"

#include "core/AABB.h"
#include "core/Matrix4.h"
#include "core/Scene.h"

#include "core/Film.h"
#include "core/Integrator.h"

namespace skirt {

int mainShared(UNUSED int argc, UNUSED char** argv) {
  // unique_ptr<Scene> scene(LoadSceneFile("example.scene"));
  unique_ptr<Scene> scene(new Scene());

  // Shape circle = ...
  // scene->SetRootElement(circle);

  unique_ptr<const Scene> final(scene->Bake(move(scene)));

  // Main thread
  Film film = final->MakeFilm();

  // // Multi thread
  unique_ptr<Integrator> integrator = final->MakeIntegrator();
  int x = 0, y = 0, width = 200, height = 100;
  FilmTile tile = integrator->Render(x, y, width, height);

  // // Main thread
  film.MergeTile(tile);
  film.SaveImage();

  return 0;
}

#ifdef __EMSCRIPTEN__

int mainEmscripten(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt emscripten";
  return 0;
}

#else

int mainNative(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt native";
  return 0;
}

#endif

}  // namespace skirt

int main(int argc, char** argv) {
  FLAGS_logtostderr = 1;
  FLAGS_v = 3;
  google::InitGoogleLogging(argv[0]);

  return skirt::mainShared(argc, argv);

#ifdef __EMSCRIPTEN__
  return skirt::mainEmscripten(argc, argv);
#else
  return skirt::mainNative(argc, argv);
#endif
}
