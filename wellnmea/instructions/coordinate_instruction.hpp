#pragma once

#include <string>
#include <cstring>
#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>

namespace wellnmea
{
  namespace instructions
  {
    /* -------------------------------------------------------------------------- */
    /*                              Coordinate value                              */
    /* -------------------------------------------------------------------------- */

    class CoordinateValue : public NullValue
    {
    protected:
      std::optional<double> base;
      double degrees_ = 0;
      double minutes_ = 0;

    public:
      CoordinateValue(const std::string &name) : NullValue(name) {}

    public:
      void setValue(double value) noexcept
      {
        base = value;
        degrees_ = std::trunc(base.value() / 100);
        minutes_ = base.value() - degrees_ * 100;
      }

      void reset() noexcept
      {
        base = std::nullopt;
      }

      std::optional<double> decimal() const
      {
        if(base.has_value()) {
          return degrees_ + minutes_ / 60.0;
        }
        return std::nullopt;
      }

      std::optional<int> degrees() const
      {
        if (base.has_value())
        {
          return degrees_;
        }
        return std::nullopt;
      }

      std::optional<double> minutes() const
      {
        if (base.has_value())
        {
          return minutes_;
        }
        return std::nullopt;
      }
    };
    /* -------------------------------------------------------------------------- */
    /*                                 Instruction                                */
    /* -------------------------------------------------------------------------- */
    class CoordinateInstruction : public Instruction
    {
    public:
      CoordinateInstruction(const std::string &name) : Instruction(name) {}

    public:
      std::string which() const noexcept override
      {
        return "coordinate";
      }

      value *extract(position it, const_position end) override
      {
        auto coordinate = new CoordinateValue(name());
        if (!it->empty())
        {
          double value = util::toDouble(*it);
          coordinate->setValue(value);
        }

        ++it;
        return coordinate;
      }

      Instruction *clone(const std::string &name) const override
      {
        return new CoordinateInstruction(name);
      }
    };

  } // namespace instructions

} // namespace wellnmea
