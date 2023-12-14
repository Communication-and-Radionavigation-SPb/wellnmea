#pragma once

#include <wellnmea/formats/format.hpp>

#include <wellnmea/instructions/latitude_instruction.hpp>
#include <wellnmea/instructions/longitude_instruction.hpp>
#include <wellnmea/instructions/number_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>
#include <wellnmea/instructions/utc_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}

/**
 * @brief  NMEA **GGA message parsing fromat
 * 
 * Example:
 * $GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F
 *            ^        ^         ^      ^         ^ ^ ^  ^    ^    ^    ^    ^  ^    ^
 *            1        2         3      4         5 6 7  8    9   10   11   12 13   14 
 * 1  - UTC timestamp
 * 2  - Latitude value
 * 3  - Latitude direction N || S (North or South)
 * 4  - Longitude value
 * 5  - Longitude direction E || W (East or West) 
 * 6  - GPS quality indicator
 *     0 -> Fix not valid
 *     1 -> GPS fix
 *     2 -> Differential GPS fix (DGNSS), SBAS, OmniSTAR VBS, Beacon, RTX in GVBS mode
 *     3 -> Not applicable
 *     4 -> RTK Fixedx, xFill
 *     5 -> RTK Float, OmniSTAR XP/HP, Location RTK, RTX
 *     6 -> INS Dead reckoning
 * 7  - Number of sattelites in view 0-24
 * 8  - Horizontal delution of position
 * 9  - Orthometric height value (MSL reference) (unsupported)
 * 10 - Orthometric height measure units (M for meters) (unsupported)
 * 11 - Geoid separation value (unsupported)
 * 12 - Geoid separation measure units (M for meters) (unsupported)
 * 13 - Age of differential GPS data record, Type 1 or Type 9, Null when DGPS is not used (unsupported)
 * 14 - Reference station ID (0 - 4095) (unsupported)
 */
class GGA : public wellnmea::formats::Format {
 public:
  GGA()
      : Format({
            // UTC time in position
            new wa::UtcInstruction("time"),
            // latitude of vessel position
            new wa::LatitudeInstruction("latitude"),
            // longitude of vessel position
            new wa::LongitudeInstruction("longitude"),
            // gps quality indicator
            new wa::RangeNumberInstruction("quality",
                                           std::make_tuple<int, int>(0, 6)),
            // number of satellites in view
            new wa::NumberInstruction("satellites"),
            // horizontal delution of position
            new wa::NumberInstruction("hdop"),
        }){};
  virtual ~GGA() = default;
};
