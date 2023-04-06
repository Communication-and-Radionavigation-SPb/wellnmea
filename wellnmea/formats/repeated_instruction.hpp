#pragma once

#include <wellnmea/formats/instruction.hpp>

namespace wellnmea
{
  namespace formats
  {
    class RepeatedInstruction: public Instruction {
      public:
        RepeatedInstruction(const std::string & name): Instruction(name) {}

      public:
        std::string which() const noexcept override {
          return "repetition";
        }

        Instruction *clone(const std::string &name) const override {
          return new RepeatedInstruction(name);
        }

        Instruction *applyParams(props params) override {
          return this;
        }

        value *extract(position it) override {
          
        }
    };
  } // namespace formats
  
} // namespace wellnmea
