#pragma once

#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/utc_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>
#include <wellnmea/instructions/latitude_instruction.hpp>
#include <wellnmea/instructions/longitude_instruction.hpp>
#include <wellnmea/instructions/speed_instruction.hpp> 
#include <wellnmea/instructions/degrees_instruction.hpp>
#include <wellnmea/instructions/date_instruction.hpp>


namespace wa {
using namespace wellnmea::instructions;
}  // namespace wa

/**
 * @brief  RMC NMEA message parsing format
 * 
 * Example:
 * $--RMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,xxxx,x.x,a*hh
 *             ^    ^     ^   ^     ^    ^  ^   ^   ^    ^  ^
 *             1    2     3   4     5    6  7   8   9   10  11
 * 
 * 1. UTC timestamp
 * 2. Status. A = Valid, V = Warning
 * 3. Latitude value
 * 4. Latitude direction. N = North, S = South
 * 5. Longitude value
 * 6. Longitude direction. W = West, E = East
 * 7. Speed in knots
 * 8. Track true
 * 9. Date in ddmmyy format
 * 10. Magnetic variation, degrees
 * 11. Magnetic variation direction. E = East, W = West
 * 
 *  
 */
class RMC : public wellnmea::formats::Format {
  public:
    RMC():
      Format({
        new wa::UtcInstruction("time"),
        new wa::SymbolInstruction("status"),
        new wa::LatitudeInstruction("latitude"),
        new wa::LongitudeInstruction("longitude"),
        new wa::SpeedInstruction("speed", 'N'),
        new wa::DegreesInstruction("track"),
        new wa::DateInstruction("date"),
      }) {}
virtual ~RMC() = default;
};
