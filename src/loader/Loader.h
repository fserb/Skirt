#pragma once

#include "core/skirt.h"

#include "core/Scene.h"

namespace skirt {

unique_ptr<Scene> LoadSceneFile(const string& filename);
unique_ptr<Scene> LoadSceneString(const string& data);

}  // namespace skirt
