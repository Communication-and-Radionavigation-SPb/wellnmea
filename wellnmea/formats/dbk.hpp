#pragma once

#include <wellnmea/formats/format.hpp>

#include <wellnmea/instructions/distance_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}  // namespace wa

class DBK : public wellnmea::formats::Format {
 public:
  DBK()
      : Format({new wa::DistanceInstruction("feet"),
                new wa::DistanceInstruction("meters"),
                new wa::DistanceInstruction("fathoms")}) {}
  virtual ~DBK() = default;
};
