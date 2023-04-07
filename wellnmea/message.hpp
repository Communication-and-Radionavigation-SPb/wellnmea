#pragma once

#include <string>
#include <map>
#include <memory>

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  using wellnmea::values::NullValue;
  class NmeaMessage
  {
  protected:
    std::map<std::string, std::shared_ptr<NullValue>> values;

  public:
    std::string talker{};
    std::string formatter{};

  public:
  };
} // namespace wellnmea
