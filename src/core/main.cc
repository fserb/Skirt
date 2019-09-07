// main

#include "core/skirt.h"

#include <stdio.h>

#include "loader/Loader.h"

namespace skirt {

#ifdef __EMSCRIPTEN__

int mainEmscripten(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt emscripten";
  return 0;
}

#else

int mainNative(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt native";

  LoadSceneFile("example.scene");

  return 0;
}

#endif

}  // namespace skirt

int main(int argc, char** argv) {
  FLAGS_logtostderr = 1;
  FLAGS_v = 3;
  google::InitGoogleLogging(argv[0]);

#ifdef __EMSCRIPTEN__
  return skirt::mainEmscripten(argc, argv);
#else
  return skirt::mainNative(argc, argv);
#endif
}
