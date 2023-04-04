#pragma once

#include <exception>
#include <string>

namespace wellnmea
{
  /**
   * @brief Invalid configuration format error
   * 
   * Error occures when there is an error in configuration
   * provided to the service.
   * 
   * Valid configuration is:
   * name_of_parameter|parameter_format[(param1[,param2[,param3]])]
   * 
   * name_of_parameter is any alfa-numeric value which have to start from
   * character or underscore sign
   * 
   * parameter format is one of predefined NMEA0183 formats:
   * - utc - utc time stamp
   * - longitude - longitude value with direction (E or W) field going next.
   * - latitude - latitude value with direction (N or S) field going next.
   * - checksum - checksum field which marks that checksum is required
   * - symbol - character field
   * - number - number field
   * - string - string field
   * 
   */
  class configuration_error: public std::exception {
    protected:
      std::string m_reason;
    public:
      configuration_error(const std::string &reason): m_reason(reason) {}
      ~configuration_error() {}
    public:
      const char* what() const noexcept override {
        return m_reason.c_str();
      }
  };
} // namespace wellnmea
