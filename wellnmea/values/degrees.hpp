#pragma once

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace values
  {
    using std::optional;
    class _DegreesValue : public NullValue
    {
    public:
      enum Measure
      {
        Magnetic,
        True,
        Unknown
      };

      static Measure typeFrom(const char &c) noexcept
      {
        if (c == 'M')
          return Magnetic;
        if (c == 'T')
          return True;
        return Unknown;
      }

      static std::string typeToString(Measure m) noexcept
      {
        if (m == Magnetic)
          return "Magnetic";
        if (m == True)
          return "True";
        return "Unknown";
      }

    public:
      _DegreesValue(const std::string &name) : NullValue(name) {}
      virtual ~_DegreesValue() = default;

      virtual optional<double> cursor() const noexcept = 0;
      virtual optional<Measure> measure() const noexcept = 0;

      virtual void accept(visitor_base &v) const noexcept override
      {
        using value_visitor = visitor<_DegreesValue>;
        value_visitor *ev = dynamic_cast<value_visitor *>(&v);
        if (ev != nullptr)
          ev->visit(this);
      }
    };

    class DegreesValue : public _DegreesValue
    {
    private:
      double m_cursor;
      Measure m_measure;

    public:
      DegreesValue(const std::string &name,
                   double cursor,
                   Measure measure) : m_cursor(cursor),
                                      m_measure(measure),
                                      _DegreesValue(name) {}

      optional<double> cursor() const noexcept override
      {
        return m_cursor;
      }
      optional<Measure> measure() const noexcept override
      {
        return m_measure;
      }
    };

    class NullDegreesValue : public _DegreesValue
    {
    public:
      NullDegreesValue(const std::string &name) : _DegreesValue(name) {}

      optional<double> cursor() const noexcept override
      {
        return std::nullopt;
      }

      optional<Measure> measure() const noexcept override
      {
        return std::nullopt;
      }
    };
  } // namespace values

} // namespace wellnmea
