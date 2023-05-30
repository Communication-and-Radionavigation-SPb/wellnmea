#pragma once

#include <string>
#include <list>
#include <memory>

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  using wellnmea::values::NullValue;
  class Message
  {
  protected:
    std::list<std::shared_ptr<NullValue>> m_values;
    std::string m_talker{};
    std::string m_formatter{};

  public:
    Message(const std::string &talker,
            const std::string formatter,
            const std::list<std::shared_ptr<NullValue>> values)
        : m_values(values),
          m_talker(talker),
          m_formatter(formatter)
    {
    }

    const std::string &talker() const noexcept
    {
      return m_talker;
    }

    const std::string &formatter() const noexcept
    {
      return m_formatter;
    }

    const std::list<std::shared_ptr<NullValue>> &values() const noexcept
    {
      return m_values;
    }
  };
} // namespace wellnmea
