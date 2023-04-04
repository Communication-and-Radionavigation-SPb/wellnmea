#pragma once

#include <string>
#include <map>
#include <memory>

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
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
