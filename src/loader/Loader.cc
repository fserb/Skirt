// loader

#include "loader/Loader.h"

#include <yaml-cpp/yaml.h>

namespace skirt {

void LoadScene(YAML::Node root) {
  for (const auto& child : root) {
    const string key = child.first.as<string>();
    const int dot = key.find('.');
    // "command.type"
    const string command = key.substr(0, dot);
    const string type = dot == -1 ? "" : key.substr(dot + 1);
  }
}

void LoadSceneFile(const string& filename) {
  YAML::Node root = YAML::LoadFile(filename);
  LoadScene(std::move(root));
}

void LoadSceneString(const string& data) {
  YAML::Node root = YAML::Load(data);
  LoadScene(std::move(root));
}

}  // namespace skirt
