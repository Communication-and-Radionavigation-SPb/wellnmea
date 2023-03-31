#pragma once

#include <exception>
#include <string>

namespace wellnmea
{
  class parse_error: public std::exception {
    protected:
      std::string m_reason;
    public:
      parse_error(const std::string &reason): m_reason(reason) {}
      ~parse_error() {}
    public:
      const char* what() const noexcept override {
        return m_reason.c_str();
      }
  };
} // namespace wellnmea
