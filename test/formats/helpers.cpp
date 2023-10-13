#include "helpers.h"

std::vector<json> getDataFromFile(const std::string& filename) {
  std::vector<json> data;
  std::ifstream input(filename);
  json j;
  input >> j;
  if (!j.is_array())
    return {};

  for (json& item : j) {
    data.push_back(item);
  }

  return data;
}
