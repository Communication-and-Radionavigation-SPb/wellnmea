#pragma once

#include <exception>
#include <string>

namespace wellnmea
{
  /**
   * @brief context_value conversion failure
   *
   * This is the error from context_value conversion
   * thrown out from one of its methods
   */
  class conversion_error : public std::exception
  {
  private:
    std::string requested_format;

  public:
    conversion_error(const std::string &f) : requested_format(f) {}
    ~conversion_error(){};

  public:
    const char *what() const noexcept override
    {
      return ("Failed to convert context_value into requested format: " + requested_format).c_str();
    }
  };
} // namespace wellnmea
