#pragma once

#include "field.hpp"

#include <functional>
#include <string>
#include <utility>
#include <list>

namespace wellnmea
{

  using std::list;
  using std::optional;
  using std::string;

  class context
  {
    /* -------------------------------------------------------------------------- */
  private:
    /// @brief  Plain income message reperesentation
    const string m_income;
    /* -------------------------------------------------------------------------- */
    /// @brief Message source identifier
    const optional<string> m_source;
    /* -------------------------------------------------------------------------- */
    /// @brief Source message type
    optional<string> m_type;
    /* -------------------------------------------------------------------------- */
    /// @brief Message parsed fields list
    list<field> fields;
    /* -------------------------------------------------------------------------- */
  public:
    context(const string &a_message) : m_income(a_message),
                                       m_source(std::nullopt),
                                       m_type(std::nullopt),
                                       fields({}) {}

  public:
    /* -------------------------------------------------------------------------- */
    /// @brief Retrieves message type
    /// @return Message type
    optional<string> type() const noexcept
    {
      return m_type;
    }
    /* -------------------------------------------------------------------------- */
    uint8_t checksum() const noexcept
    {
    }
  };
} // namespace wellnmea
