#include "core/skirt.h"

#include "loader/parser.h"

namespace skirt {

void LoadSceneError();

string lower(const string& orig) {
  string ret;
  ret.resize(orig.length());
  transform(orig.begin(), orig.end(), ret.begin(), ::tolower);
  return ret;
}

void error(const string error, const YAML::Node& node) {
  LOG(FATAL) << node.Mark().line << ":" << node.Mark().column << ": " << error
             << ":'" << node.as<string>() << "'";
  LoadSceneError();
}

void assertMap(const YAML::Node& node) {
  if (!node.IsMap()) error("Not a map", node);
}

void assertSequence(const YAML::Node& node, size_t size) {
  if (!node.IsSequence() || node.size() != size) error("Not a vector", node);
}

void assertFloat(const YAML::Node& node) {
  if (!node.IsScalar()) error("Not a value", node);
  node.as<float>();
}

void assertInt(const YAML::Node& node) {
  if (!node.IsScalar()) error("Not a value", node);
  node.as<int>();
}

void assertString(const YAML::Node& node) {
  if (!node.IsScalar()) error("Not a string", node);
}

Vector3 parseVector3(const YAML::Node& node) {
  assertSequence(node, 3);
  Vector3 ret;

  for (int i = 0; i < 3; ++i) {
    assertFloat(node[i]);
    ret[i] = node[i].as<float>();
  }

  return ret;
}

float parseFloat(const YAML::Node& node) {
  assertFloat(node);
  return node.as<float>();
}

int parseInt(const YAML::Node& node) {
  assertInt(node);
  return node.as<int>();
}

string parseString(const YAML::Node& node) {
  assertString(node);
  return node.as<string>();
}

}  // namespace skirt
