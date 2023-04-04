#pragma once

#include <exception>
#include <string>

namespace wellnmea
{
  /**
   * @brief Invalid instruction parameter error
   */
  class invalid_param: public std::exception {
    protected:
      std::string m_reason;
    public:
      invalid_param(const std::string &reason): m_reason(reason) {}
      ~invalid_param() {}
    public:
      const char* what() const noexcept override {
        return m_reason.c_str();
      }
  };
} // namespace wellnmea
