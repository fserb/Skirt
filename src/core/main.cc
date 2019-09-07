// main

#include "core/skirt.h"

#include <stdio.h>

int mainNative(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt native";
  return 0;
}

int mainEmscripten(UNUSED int argc, UNUSED char** argv) {
  DVLOG(1) << "Skirt emscripten";
  return 0;
}

int main(int argc, char** argv) {
  FLAGS_logtostderr = 1;
  FLAGS_v = 3;
  google::InitGoogleLogging(argv[0]);

#ifdef __EMSCRIPTEN__
  return mainEmscripten(argc, argv);
#else
  return mainNative(argc, argv);
#endif
}
