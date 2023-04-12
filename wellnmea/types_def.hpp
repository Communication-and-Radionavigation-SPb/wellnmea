#pragma once

#include <variant>
#include <map>
#include <any>
#include <optional>
#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  using SerializedProperty = std::map<std::string, std::any>;

  using SerializedResult = std::optional<SerializedProperty>;
} // namespace wellnmea
