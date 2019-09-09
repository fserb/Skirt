#pragma once

#include <yaml-cpp/yaml.h>

#include "core/skirt.h"

namespace skirt {

string lower(const string& orig);

void error(const string error, const YAML::Node& node);

void assertSequence(const YAML::Node& node, size_t size);
void assertNumber(const YAML::Node& node);
void assertString(const YAML::Node& node);
void assertMap(const YAML::Node& node);

Vector3 parseVector3(const YAML::Node& node);

}  // namespace skirt
