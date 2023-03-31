#pragma once

#include <string>
#include <exception>

namespace wellnmea
{
  class format_unregistered : public std::exception
  {
  private:
    std::string m_requested;

  public:
    format_unregistered(const std::string &fmtname) : exception(), m_requested(fmtname) {}

    const char *what() const noexcept override
    {
      return "Requested format have not been registered";
    }

    const char *get_name() const noexcept
    {
      return m_requested.c_str();
    }
  };
} // namespace wellnmea
