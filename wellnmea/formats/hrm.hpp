#pragma once

#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/degrees_instruction.hpp>
#include <wellnmea/instructions/number_instruction.hpp>
#include <wellnmea/instructions/skip_empty_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}

/**
 * @brief Roll measurement format
 * 
 * Example:
 * $--HRM,x.x,x.x,x.x,x.x,a,x.x,x.x,hhmmss.ss,xx,xx,xxxx,x.x,a*hh
 *         ^   ^   ^   ^  ^  ^   ^      ^     ^  ^   ^    ^  ^
 *         1   2   3   4  5  6   7      8     9  10  11   12 13
 * 
 * 1. Actual roll angle, degrees
 * 2. Roll period, seconds
 * 3. Roll amplitude, degrees
 * 4. null field
 * 5. Status, A=data valid, V=data invlid
 * 6. null
 * 7. null
 * 8. null
 * 9. null
 * 10. null
 * 11. null
 * 12. Heel angle alert treshhold value 
 * 13. sentence status flag
 */
class HRM : public wellnmea::formats::Format {
 public:
  HRM()
      : Format({new wa::NumberInstruction("roll"),
                new wa::NumberInstruction("period"),
                new wa::NumberInstruction("amplitude"),
                new wa::SkipInstruction(1),
                new wa::SymbolInstruction("status", {'A'}),
                new wa::SkipInstruction(6), new wa::NumberInstruction("heel"),
                new wa::SymbolInstruction("status")}) {}
};