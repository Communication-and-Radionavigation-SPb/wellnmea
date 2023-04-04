#pragma once

#include <cctype>
#include <stack>
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

    std::string::const_iterator extract_between(
        const char &opening,
        const char &closing,
        std::string::const_iterator start,
        std::string::const_iterator end) noexcept
    {
      int st = 0;
      for (auto it = start; it != end; it++)
      {
        if(*it == opening) {
          st++;
        } else if (*it == closing) {
          if(--st == 0) {
            return it;
          }
        }
      }
      return end;
    }
  } // namespace util

} // namespace wellnmea
