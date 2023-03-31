#pragma once

#include <cctype>
#include <string>
#include <algorithm>

namespace wellnmea
{
  namespace util
  {
    std::string to_lower(std::string &source) noexcept
    {
      std::string clone(source);
      std::transform(source.begin(), source.end(), clone.begin(), [](unsigned char c)
                     { return std::tolower(c); });
      return clone;
    }
  } // namespace util

} // namespace wellnmea
