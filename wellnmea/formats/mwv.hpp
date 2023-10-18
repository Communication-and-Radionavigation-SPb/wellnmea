#pragma once

#include <wellnmea/formats/format.hpp>

#include <wellnmea/instructions/degrees_instruction.hpp>
#include <wellnmea/instructions/speed_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}  // namespace wa

/**
 * @brief NMEA MWV message parsing format
 * 
 * Example:
 *  $WIMWV,80.8,R,40.5,N,A*22
 *           ^  ^   ^  ^ ^
 *           1  2   3  4 5
 * 1 - Wind angle, 0 to 359 degrees
 * 2 - Reference, R = Relative, T = Theoretical(not used)
 * 3 - Wind speed
 * 4 - Wind speed units, K = kilometers/hour, M = meters/hour, N = knots
 * 5 - Status, A = Data Valid, V = Invalid
 * 
 */
class MWV : public wellnmea::formats::Format {
 public:
  MWV()
      : Format({
            new wa::DegreesInstruction("angle"),
            new wa::SymbolInstruction("reference"),
            new wa::SpeedInstruction("speed"),
            new wa::SymbolInstruction("status"),
        }) {}
};
