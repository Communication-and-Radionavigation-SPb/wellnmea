#pragma once

#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/degrees_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

using namespace wellnmea;

/**
 * @brief Nmea ROT message parsing format
 * 
 * Example:
 * $GPROT,3.8,A*3A
 *        ^   ^
 *        1   2
 * 1. Rate of turn value (in degrees)
 * 2. Status ('A' for valid)
 */
class ROT : public formats::Format {
 public:
  ROT()
      : Format({new instructions::DegreesInstruction("degrees"),
                new instructions::SymbolInstruction("status", {'A'})}) {}
  virtual ~ROT() = default;
};