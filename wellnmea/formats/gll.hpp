#pragma once

#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/latitude_instruction.hpp>
#include <wellnmea/instructions/longitude_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>
#include <wellnmea/instructions/utc_instruction.hpp>

using namespace wellnmea;

/**
 * @brief Nmea GLL message parsing format
 * 
 * Example:
 * $GPGLL,3953.88008971,N,10506.75318910,W,034138.00,A,D*7A
 *            ^         ^      ^         ^      ^    ^ ^
 *            1         2      3         4      5    6 7
 * 1. Latitude value
 * 2. Latitude direction N|S (Noth or South)
 * 3. Longitude value
 * 4. Longitude direction E|W (East or West)
 * 5. UTC timestamp hhmmss.ss
 * 6. Status indicator
 *    - A = data valid
 *    - V = data invalid
 * 7. Mode indicator
 *    - A = Autonomous mode
 *    - D = Differential mode
 *    - E = Estimated (dead reckoning) mode
 *    - M = Manual input mode 
 *    - S = Simulator mode
 *    - N = Data not valid
 */
class GLL : public formats::Format {
 public:
  GLL()
      : Format({new instructions::LatitudeInstruction("latitude"),
                new instructions::LongitudeInstruction("longitude"),
                new instructions::UtcInstruction("time"),
                new instructions::SymbolInstruction("status", {'A', 'V'}),

                new instructions::SymbolInstruction(
                    "mode", {'A', 'D', 'E', 'M', 'S', 'N'})}){};
  virtual ~GLL() = default;
};