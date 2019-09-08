// main

#include "core/skirt.h"

#include <stdio.h>

#include "loader/Loader.h"

#include "core/AABB.h"
#include "core/Matrix4.h"
#include "core/Scene.h"

#include "core/Film.h"

namespace skirt {

#ifdef __EMSCRIPTEN__

int mainEmscripten(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt emscripten";
  return 0;
}

#else

int mainNative(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt native";

  Film f(200, 100, "test.exr");
  FilmTile t(0, 0, 200, 100);

  for (int y = 0; y < 100; ++y) {
    for (int x = 0; x < 200; ++x) {
      Vector3 c(x / 200.0, y / 100.0, 0.2);
      t.WritePixel(x, y, c);
    }
  }

  f.MergeTile(t);
  f.SaveImage();

  // unique_ptr<Scene> scene(LoadSceneFile("example.scene"));
  // unique_ptr<Scene> scene(new Scene());

  // unique_ptr<const Scene> final(scene->Bake(move(scene)));

  // Main thread
  // Film* film = Scene->Film();
  // // Multi thread
  // Integrator* integrator = final->Integrator();
  // FilmTile* tile = integrator->Render({x, y, width, height});
  // // Main thread
  // film->MergeTile(tile);
  // film->SaveImage();

  return 0;
}

#endif

}  // namespace skirt

int main(int argc, char** argv) {
  FLAGS_logtostderr = 1;
  FLAGS_v = 3;
  google::InitGoogleLogging(argv[0]);

  skirt::Vector3 a(1, 2, 3);
  DVLOG(1) << a;

#ifdef __EMSCRIPTEN__
  return skirt::mainEmscripten(argc, argv);
#else
  return skirt::mainNative(argc, argv);
#endif
}
