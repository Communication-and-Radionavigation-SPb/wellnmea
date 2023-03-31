#pragma once

#include <string>

namespace wellnmea
{
  class param
  {
  private:
    std::string m_name;

  public:
    /**
     * @brief Construct a new param object
     *
     * @param a_name Name of parameter
     */
    param(std::string a_name);
    virtual ~param() = 0;

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

  param::param(std::string a_name) : m_name(a_name)
  {
  }

  param::~param()
  {
  }

} // namespace wellnmea
