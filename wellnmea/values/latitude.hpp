#pragma once

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace values
  {
    using std::optional;
    class _LatitudeValue : public NullValue
    {
    public:
      enum Direction
      {
        North,
        South,
        Unknown,
      };
      static Direction directionFrom(const char &c) noexcept
      {
        if (c == 'N')
          return Direction::North;
        if (c == 'S')
          return Direction::South;
        return Direction::Unknown;
      }

      static std::string directionToString(Direction d) noexcept
      {
        if (d == North)
          return "North";
        if (d == South)
          return "South";
        return "Unknown";
      }

    public:
      _LatitudeValue(const std::string &name) : NullValue(name) {}
      virtual ~_LatitudeValue() = default;

      virtual optional<double> position() const noexcept = 0;
      virtual optional<Direction> direction() const noexcept = 0;

      virtual void accept(visitor_base &v) const noexcept
      {
        using value_visitor = visitor<_LatitudeValue>;
        if (value_visitor *ev = dynamic_cast<value_visitor *>(&v))
          ev->visit(this);
      }
    };

    class LatitudeValue : public _LatitudeValue
    {
    private:
      double m_coordinate;
      Direction m_direction;

    public:
      LatitudeValue(const std::string &name,
                    double coordinate,
                    Direction direction) : m_coordinate(coordinate),
                                           m_direction(direction),
                                           _LatitudeValue(name) {}

      optional<double> position() const noexcept override
      {
        return m_coordinate;
      }

      optional<Direction> direction() const noexcept override
      {
        return m_direction;
      }
    };

    class NullLatitudeValue : public _LatitudeValue
    {
    public:
      NullLatitudeValue(const std::string &name) : _LatitudeValue(name) {}

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
