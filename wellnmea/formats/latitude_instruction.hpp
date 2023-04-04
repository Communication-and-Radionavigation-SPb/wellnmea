#pragma once

#include <wellnmea/formats/instruction.hpp>
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

      virtual Instruction *applyParams(props params)
      {
        return this;
      }
    };
  } // namespace formats

} // namespace wellnmea
