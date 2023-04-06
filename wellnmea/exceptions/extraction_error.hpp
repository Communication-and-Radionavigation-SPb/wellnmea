#pragma once

#include <exception>
#include <string>

namespace wellnmea
{
  /**
   * @brief Failure representing format instruction
   * evaluation error.
   * 
   * Commonly happens when type of token not matches
   * required one.
   * 
   */
  class extraction_error: public std::exception {
    protected:
      std::string m_reason;
    public:
      extraction_error(const std::string &reason): m_reason(reason) {}
      ~extraction_error() {}
    public:
      const char* what() const noexcept override {
        return m_reason.c_str();
      }
  };
} // namespace wellnmea
