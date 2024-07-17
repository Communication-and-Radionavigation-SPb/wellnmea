#pragma once

#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/as_is_instruction.hpp>
#include <wellnmea/instructions/number_instruction.hpp>
#include <wellnmea/instructions/repeated_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

namespace wa {
using namespace wellnmea::instructions;
}  // namespace wa

class XDR : public wellnmea::formats::Format {
 public:
  XDR()
      : Format({new wa::RepeatedInstruction(
            "measurement", {new wa::SymbolInstruction("type"),
                            new wa::NumberInstruction("value"),
                            new wa::SymbolInstruction("units"),
                            new wa::AsIsInstruction("transduser")})}) {}
  virtual ~XDR() = default;
};
