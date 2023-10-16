#pragma once

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>
#include <wellnmea/util/string_utils.hpp>

namespace wellnmea
{
  namespace instructions
  {
    /* -------------------------------------------------------------------------- */
    /*                                  Utc value                                 */
    /* -------------------------------------------------------------------------- */

    class UtcValue : public NullValue
    {
    public:
      using visitor_t = visitor<UtcValue>;

    protected:
      std::optional<double> timestamp;

      int hours = 0;
      int minutes = 0;
      int seconds = 0;
      int milliseconds = 0;

    public:
      UtcValue(const std::string& name) : NullValue(name) {}

    public:
      virtual void accept(visitor_base &v) const noexcept override
      {
      }

      void reset() noexcept {
        timestamp = std::nullopt;
      }

      std::optional<double> getTimestamp() const noexcept
      {
        return timestamp;
      }

      void setTimestamp(double a_timestamp) noexcept
      {
        timestamp = a_timestamp;
        hours = std::trunc(a_timestamp / 10000.0);
        minutes = std::trunc((a_timestamp - hours * 10000) / 100.0);
        seconds = a_timestamp - hours * 10000 - minutes * 100;
        milliseconds = std::trunc((a_timestamp - (int) a_timestamp) * 1000);
        //                                     ^                     ^
        //                             get only decimal part    move to integers
      }

      std::optional<int> getHours() const noexcept
      {
        if (timestamp.has_value())
        {
          return hours;
        }
        return std::nullopt;
      }

      std::optional<int> getMinutes() const noexcept
      {
        if (timestamp.has_value())
        {
          return minutes;
        }
        return std::nullopt;
      }

      std::optional<int> getSeconds() const noexcept
      {
        if (timestamp.has_value())
        {
          return seconds;
        }
        return std::nullopt;
      }

      std::optional<int> getMilliseconds() const noexcept
      {
        if(timestamp.has_value())
        {
          return milliseconds;
        }
        return std::nullopt;
      }
    };

    /* -------------------------------------------------------------------------- */
    /*                                 Instruction                                */
    /* -------------------------------------------------------------------------- */

    class UtcInstruction : public Instruction
    {
    public:
      UtcInstruction(const std::string &name) : Instruction(name) {}

    public:
      std::string which() const noexcept override
      {
        return "utc";
      }

      value *extract(position it, const_position end) override
      {
        auto value = new UtcValue(name());

        if (!it->empty())
        {
          double timestamp = util::toDouble(*it);
          value->setTimestamp(timestamp);
        }

        it++;
        return value;
      }

      Instruction *clone(const std::string &name) const override
      {
        return new UtcInstruction(name);
      }
    };
  } // namespace instructions

} // namespace wellnmea
