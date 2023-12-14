#pragma once

#include <wellnmea/formats/format.hpp>

#include <wellnmea/instructions/distance_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}  // namespace wa
/**
 * @brief NMEA **DPT message parsing format** - Depth of Water
 *  
 * Example:
 * $INDPT,2.3,0.0*46 
 *         ^   ^
 *         1   2
 * 1  - Depth relative to transducer, meters
 * 2  - Offset from transducer, meters
 *      Positive means distance from transducer to water line
 *      Negative means distance from transducer to keel
 */
class DPT : public wellnmea::formats::Format {
 public:
  DPT()
      : Format({
            new wa::DistanceInstruction("depth", 'M'),
            new wa::DistanceInstruction("offset", 'M'),
        }) {}
  virtual ~DPT() = default;
};