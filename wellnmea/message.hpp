#pragma once

#include <string>
#include <map>
#include <memory>

#include <wellnmea/params/param.hpp>

namespace wellnmea
{
  class NmeaMessage
  {
  protected:
    std::map<std::string, std::shared_ptr<param>> params;

  public:
    std::string talker{};
    std::string formatter{};

  public:
  };
} // namespace wellnmea
