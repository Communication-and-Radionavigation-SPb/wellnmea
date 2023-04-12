#pragma once

#include <optional>

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace values
  {
    using std::optional;

    class _LongitudeValue : public NullValue
    {
    public:
      enum Direction
      {
        East,
        West,
        Unknown
      };
      static Direction directionFrom(const char &c) noexcept
      {

        if (c == 'E')
          return East;
        if (c == 'W')
          return West;
        return Unknown;
      }
      static std::string directionToString(Direction d)
      {
        if (d == East)
          return "East";
        if (d == West)
          return "West";
        return "Unknown";
      }

    public:
      _LongitudeValue(const std::string &name) : NullValue(name) {}
      virtual ~_LongitudeValue() {}
      virtual optional<double> position() const noexcept = 0;
      virtual optional<Direction> direction() const noexcept = 0;

      virtual void accept(visitor_base &v) const noexcept
      {
        using value_visitor = visitor<_LongitudeValue>;
        if (value_visitor *ev = dynamic_cast<value_visitor *>(&v))
          ev->visit(this);
      }
    };

    class LongitudeValue : public _LongitudeValue
    {
    private:
      double m_coordinate;
      Direction m_direction;

    public:
      LongitudeValue(const std::string &name,
                     double coordinate,
                     Direction direction) : m_coordinate(coordinate),
                                            m_direction(direction),
                                            _LongitudeValue(name) {}

      optional<double> position() const noexcept override
      {
        return m_coordinate;
      }

      optional<Direction> direction() const noexcept override
      {
        return m_direction;
      }
    };

    class NullLongitudeValue : public _LongitudeValue
    {
    public:
      NullLongitudeValue(const std::string &name) : _LongitudeValue(name) {}

      optional<double> position() const noexcept override
      {
        return std::nullopt;
      }

      optional<Direction> direction() const noexcept override
      {
        return std::nullopt;
      }
    };
  } // namespace values

} // namespace wellnmea
