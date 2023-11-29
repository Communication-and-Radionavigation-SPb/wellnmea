#pragma once

#include <wellnmea/formats/format.hpp>

#include <wellnmea/instructions/number_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}  // namespace wa

class DPT : public wellnmea::formats::Format {
 public:
  DPT()
      : Format({
            new wa::NumberInstruction("depth"),
            new wa::NumberInstruction("offset"),
        }) {}
};