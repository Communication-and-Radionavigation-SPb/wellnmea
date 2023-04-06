#pragma once

#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/latitude.hpp>
#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace formats
  {
    class LatitudeInstruction : public Instruction
    {
    public:
      LatitudeInstruction(const std::string &name) : Instruction(name) {}

    public:
      virtual std::string which() const noexcept override
      {
        return "latitude";
      }

      virtual Instruction *clone(const std::string &name) const override
      {
        return new LatitudeInstruction(name);
      }


      virtual value *extract(position it, const_position end) override
      {
        auto cit = it++, dit = it++;
        if (cit->type == Token::null)
        {
          return new values::NullLatitudeValue(name());
        }

        if (cit->type != Token::number)
          throw extraction_error("Failed to extract longitude field near `" + it->slice + "`");
        if (dit->type != Token::symbol)
          throw extraction_error("Failed to extract longitude direction near`" + it->slice + "`");

        double content = std::stod(cit->slice);
        values::LatitudeValue::Direction direction =
            values::LatitudeValue::directionFrom(dit->slice[0]);

        return new values::LatitudeValue(name(), content, direction);
      }
    };
  } // namespace formats

} // namespace wellnmea
