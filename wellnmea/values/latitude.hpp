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
    static Direction directionFrom(const char &c) noexcept {
      if(c == 'N') return Direction::North;
      if(c == 'S') return Direction::South;
      return Direction::Unknown;
    }
    public:
      _LatitudeValue(const std::string &name) : NullValue(name) {}
      virtual ~_LatitudeValue() = default;

      virtual optional<double> position() const noexcept = 0;
      virtual optional<Direction> direction() const noexcept = 0;
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

      SerializedResult serialise() const noexcept override
      {
        return SerializedProperty({});
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

      SerializedResult serialise() const noexcept override
      {
        return std::nullopt;
      }
    };
  } // namespace values

} // namespace wellnmea