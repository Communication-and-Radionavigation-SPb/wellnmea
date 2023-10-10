#pragma once

#include <string>
#include <optional>
#include <wellnmea/values/null_value.hpp>

using std::optional;
using std::string;

namespace wellnmea
{
  namespace values
  {
    class _LetterValue : public NullValue
    {
    public:
      _LetterValue(const string &name) : NullValue(name) {}

      virtual ~_LetterValue() = default;

    public:
      virtual optional<char> value() const noexcept = 0;

      virtual void accept(visitor_base &v) const noexcept override
      {
        using value_visitor = visitor<_LetterValue>;
        value_visitor *ev = dynamic_cast<value_visitor *>(&v);
        if (ev != nullptr)
        {
          ev->visit(this);
        }
      }
    };

    class NullLetterValue : public _LetterValue
    {
    public:
      NullLetterValue(const string &name) : _LetterValue(name) {}

    public:
      optional<char> value() const noexcept override
      {
        return std::nullopt;
      }
    };

    class LetterValue : public _LetterValue
    {
    protected:
      char m_value;

    public:
      LetterValue(const string &name, char ch) : _LetterValue(name), m_value(ch) {}

    public:
      optional<char> value() const noexcept override
      {
        return m_value;
      }
    };

  } // namespace values

} // namespace wellnmea
