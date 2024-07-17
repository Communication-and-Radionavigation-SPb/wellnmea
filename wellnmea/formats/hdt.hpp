
#pragma once

#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/degrees_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

class Hdt : public wellnmea::formats::Format {
 public:
  Hdt()
      : Format({new wellnmea::instructions::DegreesInstruction("degrees"),
                new wellnmea::instructions::SymbolInstruction("measure",
                                                              {'T', 'M'})}) {}
  virtual ~Hdt() = default;
};