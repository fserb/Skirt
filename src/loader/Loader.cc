// loader

#include "loader/Loader.h"

#include <yaml-cpp/yaml.h>

#include "core/Scene.h"

namespace skirt {

unique_ptr<Scene> LoadScene(YAML::Node root) {
  unique_ptr<Scene> scene(new Scene());
  for (const auto& child : root) {
    const string key = child.first.as<string>();
    const int dot = key.find('.');
    // "command.type"
    const string command = key.substr(0, dot);
    const string type = dot == -1 ? "" : key.substr(dot + 1);
  }

  return scene;
}

unique_ptr<Scene> LoadSceneFile(const string& filename) {
  YAML::Node root = YAML::LoadFile(filename);
  return LoadScene(std::move(root));
}

unique_ptr<Scene> LoadSceneString(const string& data) {
  YAML::Node root = YAML::Load(data);
  return LoadScene(std::move(root));
}

}  // namespace skirt
