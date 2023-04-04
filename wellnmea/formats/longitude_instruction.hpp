#pragma once

#include <wellnmea/formats/instruction.hpp>

namespace wellnmea
{
  namespace formats
  {
    class LongitudeInstruction : public Instruction
    {
    public:
      LongitudeInstruction(const std::string &name) : Instruction(name) {}

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
    };
  } // namespace formats

} // namespace wellnmea
