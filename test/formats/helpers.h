#pragma once

#if !defined(FORMATS_HELPERS)
#define FORMATS_HELPERS

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;
/* ----------------------------- Data generator ----------------------------- */
std::vector<json> getDataFromFile(const std::string& filename);
#endif  // FORMATS_HELPERS
