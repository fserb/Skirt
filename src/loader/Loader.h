#pragma once

#include "core/skirt.h"

#include "core/Scene.h"

namespace skirt {

struct LoaderState {
  unique_ptr<Scene> scene;
  Vector3 lookAtFrom;
  Vector3 lookAtTo;
};

unique_ptr<Scene> LoadSceneFile(const string& filename);
unique_ptr<Scene> LoadSceneString(const string& data);

unique_ptr<LoaderState> GetLoaderStateForTesting();

}  // namespace skirt
