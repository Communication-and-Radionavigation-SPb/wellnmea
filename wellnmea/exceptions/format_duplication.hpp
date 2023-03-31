#pragma once

#include <sstream>
#include <string>
#include <exception>

namespace wellnmea
{
  class format_duplication : public std::exception
  {
    std::string m_format_name;

  public:
    format_duplication(std::string fmt) : m_format_name(fmt), exception() {}

    const char *what() const noexcept override
    {
      return "Duplicated format names are not allowed.";
    }

    const char *get_name() const noexcept
    {
      return m_format_name.c_str();
    }
  };
} // namespace wellnmea
