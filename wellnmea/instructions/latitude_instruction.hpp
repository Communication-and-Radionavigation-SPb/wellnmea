#pragma once

#include <wellnmea/instructions/instruction.hpp>

#include <wellnmea/instructions/coordinate_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

namespace wellnmea
{
  namespace instructions
  {
    /* -------------------------------------------------------------------------- */
    /*                               Latitude value                               */
    /* -------------------------------------------------------------------------- */
    class LatitudeValue : public CoordinateValue
    {
    protected:
      std::optional<char> direction_;

    public:
      LatitudeValue(const std::string &name) : CoordinateValue(name) {}

    public:

      void accept(visitor_base &v) const noexcept override {
        using value_visitor = visitor<LatitudeValue>;
        if(value_visitor *ev = dynamic_cast<value_visitor*>(&v))
          ev->visit(this);
      }

      void setBase(std::optional<double> value) noexcept override
      {
        if (value.has_value() && direction_.value_or('x') == 'S')
        {
          value = value.value() * -1;
        }
        CoordinateValue::setBase(value);
      }

      void setDirection(std::optional<char> a_direction)
      {
        // Assign new value
        direction_ = a_direction;
        setBase(base);
      }

      std::optional<char> direction() const
      {
        return direction_;
      }

      void reset() noexcept override
      {
        CoordinateValue::reset();
        direction_ = std::nullopt;
      }

    public:
      LatitudeValue &operator=(const CoordinateValue &other)
      {
        static_cast<CoordinateValue &>(*this) = other;
        return *this;
      }
    };
    /* -------------------------------------------------------------------------- */
    /*                            Latitiude instruction                           */
    /* -------------------------------------------------------------------------- */

    class LatitudeInstruction : public Instruction
    {
    protected:
      CoordinateInstruction *coord;
      SymbolInstruction *direction;

    public:
      LatitudeInstruction(const std::string &name)
          : Instruction(name),
            coord(new CoordinateInstruction("latitude")),
            direction(new SymbolInstruction("direction", {'N', 'S'}))
      {
      }

    public:
      std::string which() const noexcept override
      {
        return "latitude";
      }

      Instruction *clone(const std::string &name) const override
      {
        return new LatitudeInstruction(name);
      }

      value *extract(position it, const_position end) override
      {
        auto result = new LatitudeValue(name());

        auto coordinate = coord->extract(it, end);
        auto dir = direction->extract(it, end)->as<CharacterValue>();

        *result = *coordinate->as<CoordinateValue>();
        result->setDirection(dir->symbol());

        return result;
      }
    };
  } // namespace instructions

} // namespace wellnmea
