#pragma once

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace values
  {
    class _UTCValue : public NullValue
    {
    public:
      _UTCValue(const std::string &name) : NullValue(name) {}
      virtual ~_UTCValue() {}

      virtual std::optional<int> hours() const noexcept = 0;
      virtual std::optional<int> minutes() const noexcept = 0;
      virtual std::optional<int> seconds() const noexcept = 0;
      virtual std::optional<int> milliseconds() const noexcept = 0;
    };
    class UTCValue : public _UTCValue
    {
    private:
      int m_hours;
      int m_minutes;
      int m_seconds;
      int m_milliseconds;

    public:
      UTCValue(const std::string &name,
               int hours,
               int minutes,
               int seconds,
               int milliseconds) : m_hours(hours), m_minutes(minutes), m_seconds(seconds),
                                   m_milliseconds(milliseconds),
                                   _UTCValue(name) {}

      std::optional<int> hours() const noexcept override
      {
        return m_hours;
      }

      std::optional<int> minutes() const noexcept override
      {
        return m_minutes;
      }

      std::optional<int> seconds() const noexcept override
      {
        return m_seconds;
      }

      std::optional<int> milliseconds() const noexcept override
      {
        return m_milliseconds;
      }

      SerializedResult serialise() const noexcept
      {
        return SerializedProperty({});
      }
    };

    class NullUTCValue : public _UTCValue
    {
    public:
      NullUTCValue(const std::string &name) : _UTCValue(name) {}

      std::optional<int> hours() const noexcept override
      {
        return std::nullopt;
      }

      std::optional<int> minutes() const noexcept override
      {
        return std::nullopt;
      }

      std::optional<int> seconds() const noexcept override
      {
        return std::nullopt;
      }

      std::optional<int> milliseconds() const noexcept override
      {
        return std::nullopt;
      }

      SerializedResult serialise() const noexcept
      {
        return std::nullopt;
      }
    };
  } // namespace params

} // namespace wellnmea
