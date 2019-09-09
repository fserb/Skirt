// loader

#include <yaml-cpp/yaml.h>

#include "core/skirt.h"

#include "core/Scene.h"
#include "loader/Loader.h"

#include "loader/parser.h"

namespace skirt {

unique_ptr<LoaderState> state;

void evalLookAt(const YAML::Node& node) {
  assertMap(node);
  for (const auto& child : node) {
    const string key = lower(child.first.as<string>());

    if (key == "from") {
      state->lookAtFrom = parseVector3(child.second);
    } else if (key == "to") {
      state->lookAtTo = parseVector3(child.second);
    } else {
      error("Invalid key", child.first);
    }
  }
}

unique_ptr<Scene> LoadScene(YAML::Node root) {
  state.reset(new LoaderState());
  state->scene.reset(new Scene());

  for (const auto& child : root) {
    assertString(child.first);
    const string key = child.first.as<string>();
    const int dot = key.find('.');
    // "command.type"
    const string command = lower(key.substr(0, dot));
    const string type = dot == -1 ? "" : lower(key.substr(dot + 1));

    DVLOG(2) << "Command: " << command << " . " << type;

    if (command == "lookat") {
      evalLookAt(child.second);
    }
  }

  return move(move(state->scene));
}

unique_ptr<Scene> LoadSceneFile(const string& filename) {
  YAML::Node root = YAML::LoadFile(filename);
  return LoadScene(std::move(root));
}

unique_ptr<Scene> LoadSceneString(const string& data) {
  YAML::Node root = YAML::Load(data);
  return LoadScene(std::move(root));
}

unique_ptr<LoaderState> GetLoaderStateForTesting() {
  return move(state);
}

}  // namespace skirt
