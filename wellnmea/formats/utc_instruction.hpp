#pragma once

#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/utc.hpp>

namespace wellnmea
{
  namespace formats
  {
    class UTCInstruction : public Instruction
    {
    public:
      UTCInstruction(const std::string &name) : Instruction(name) {}

    public:
      virtual std::string which() const noexcept override
      {
        return "utc";
      }

      virtual Instruction *clone(const std::string &name) const override
      {
        return new UTCInstruction(name);
      }

      virtual Instruction *applyParams(props params)
      {
        return this;
      }

      virtual value extract(position it) override
      {
        auto v = std::make_shared<values::UTCValue>(name());
        it++;
        return v;
      }
    };
  } // namespace formats

} // namespace wellnmea
