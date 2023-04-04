#pragma once

#include <string>

namespace wellnmea
{
  namespace values
  {
    class NullValue
    {
    private:
      std::string m_name;

    public:
      /**
       * @brief Construct a new BaseValue object
       *
       * @param a_name Name of parameter
       */
      NullValue(const std::string &a_name) : m_name(a_name){};
      ~NullValue() = default;

    public:
      /**
       * @brief Name of parameter
       *
       * @return std::string_view
       */
      std::string_view name() const noexcept
      {
        return m_name;
      }
    };


  } // namespace params

} // namespace wellnmea
