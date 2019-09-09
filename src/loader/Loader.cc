// loader

#include <yaml-cpp/yaml.h>

#include "core/skirt.h"

#include "core/Scene.h"
#include "loader/Loader.h"

#include "loader/parser.h"

namespace skirt {

Description* desc;
bool loaderError = false;

void evalWorld(const YAML::Node& node) {
  assertSequence(node);
  for (const auto& child : node) {
    const string key = lower(child.first.as<string>());
  }
}

void evalLookAt(const YAML::Node& node) {
  assertMap(node);
  for (const auto& child : node) {
    const string key = lower(child.first.as<string>());

    if (key == "from") {
      desc->lookAtFrom = parseVector3(child.second);
    } else if (key == "to") {
      desc->lookAtTo = parseVector3(child.second);
    } else {
      error("Invalid key", child.first);
    }
  }
}

void evalCamera(const string& type, const YAML::Node& node) {
  assertMap(node);
  desc->cameraType = type;
  for (const auto& child : node) {
    const string key = lower(child.first.as<string>());

    if (key == "fov") {
      desc->cameraFOV = parseFloat(child.second);
    } else if (key == "aperture") {
      desc->cameraAperture = parseFloat(child.second);
    } else if (key == "focusdistance") {
      desc->cameraFocusDistance = parseFloat(child.second);
    } else {
      error("Invalid key", child.first);
    }
  }
}

void evalIntegrator(const string& type, const YAML::Node& node) {
  desc->integratorType = type;
  for (const auto& child : node) {
    error("Invalid key", child.first);
  }
}

void evalFilm(const string& type, const YAML::Node& node) {
  assertMap(node);
  desc->filmType = type;
  for (const auto& child : node) {
    const string key = lower(child.first.as<string>());

    if (key == "filename") {
      desc->filmFilename = parseString(child.second);
    } else if (key == "resolution") {
      assertSequence(child.second, 2);
      assertInt(child.second[0]);
      assertInt(child.second[1]);
      desc->width = parseInt(child.second[0]);
      desc->height = parseInt(child.second[1]);
    } else {
      error("Invalid key", child.first);
    }
  }
}

unique_ptr<Scene> LoadScene(YAML::Node root) {
  unique_ptr<Scene> scene(new Scene());
  scene->desc.reset(new Description);
  desc = scene->desc.get();
  loaderError = false;

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
    } else if (command == "camera") {
      evalCamera(type, child.second);
    } else if (command == "integrator") {
      evalIntegrator(type, child.second);
    } else if (command == "film") {
      evalFilm(type, child.second);
    } else if (command == "world") {
      evalWorld(child.second);
    }

    if (loaderError) break;
  }

  if (loaderError) return nullptr;

  desc = nullptr;
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

void LoadSceneError() {
  loaderError = true;
}

}  // namespace skirt
