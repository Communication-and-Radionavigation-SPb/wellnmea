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
    class _NumberValue : public NullValue
    {
    public:
      _NumberValue(const string &name) : NullValue(name) {}

      virtual ~_NumberValue() = default;

    public:
      virtual optional<double> value() const noexcept = 0;

      virtual void accept(visitor_base &v) const noexcept override
      {
        using value_visitor = visitor<_NumberValue>;
        value_visitor *ev = dynamic_cast<value_visitor *>(&v);
        if (ev != nullptr)
        {
          ev->visit(this);
        }
      }
    };

    class NumberValue : public _NumberValue
    {
    private:
      double m_value;

    public:
      NumberValue(const string &name, double value) : _NumberValue(name) , m_value(value) {}

    public:
      optional<double> value() const noexcept override
      {
        return m_value;
      }
    };

    class NullNumberValue : public _NumberValue
    {
    public:
      NullNumberValue(const string &name) : _NumberValue(name) {}

    public:
      optional<double> value() const noexcept override
      {
        return std::nullopt;
      }
    };

  } // namespace values

} // namespace wellnmea
