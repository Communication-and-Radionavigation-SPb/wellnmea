#pragma once

#include <string>

#include <wellnmea/exceptions.hpp>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/longitude.hpp>

namespace wellnmea
{
  namespace formats
  {
    class LongitudeInstruction : public Instruction
    {
    public:
      LongitudeInstruction(const std::string &name) : Instruction(name) {}

    protected:
    public:
      virtual std::string which() const noexcept override
      {
        return "longitude";
      }

      virtual Instruction *clone(const std::string &name) const override
      {
        return new LongitudeInstruction(name);
      }

      virtual Instruction *applyParams(props params)
      {
        return this;
      }

      virtual value *extract(position it) override
      {
        auto cit = it++, dit = it++;

        if (cit->type == Token::null || dit->type == Token::null)
          return new values::NullLongitudeValue(name());

        if (cit->type != Token::number)
          throw extraction_error("Failed to extract longitude field near `" + it->slice + "`");
        if (dit->type != Token::symbol)
          throw extraction_error("Failed to extract longitude direction near`" + it->slice + "`");

        double content = std::stod(cit->slice);
        auto direction = values::LongitudeValue::directionFrom(dit->slice[0]);

        return new values::LongitudeValue(name(), content, direction);
      }
    };
  } // namespace formats

} // namespace wellnmea
