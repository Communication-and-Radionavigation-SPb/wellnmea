#pragma once

#include <wellnmea/formats/format.hpp>

#include <wellnmea/instructions/degrees_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}  // namespace wa

class DID : public wellnmea::formats::Format {
 public:
  DID()
      : Format({new wa::DegreesInstruction("pitch"),
                new wa::DegreesInstruction("roll"),
                new wa::DegreesInstruction("heading")}) {}
  virtual ~DID() = default;
};
