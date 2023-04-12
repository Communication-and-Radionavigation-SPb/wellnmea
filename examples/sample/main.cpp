#include <wellnmea/wellnmea.hpp>
#include <wellnmea/formats/degreese_instruction.hpp>
#include <wellnmea/formats/utc_instruction.hpp>
#include <wellnmea/formats/longitude_instruction.hpp>
#include <wellnmea/formats/latitude_instruction.hpp>

#include <iostream>

#include "print_out.h"

using namespace wellnmea;
using namespace wellnmea::formats;

int main(int argc, char* argv[]) {
  std::cout << "Sample example output";

  FormatRegistry::clear();
  InstructionsRegistry::clear();

  InstructionsRegistry::add(std::make_shared<DegreesInstruction>(""));
  InstructionsRegistry::add(std::make_shared<LongitudeInstruction>(""));
  InstructionsRegistry::add(std::make_shared<LatitudeInstruction>(""));
  InstructionsRegistry::add(std::make_shared<UTCInstruction>(""));
  
  DefaultFormatBuilder builder;
  auto dtmLexems = FormatInterpreter::interpret("heading|degrees;checksum|checksum(true)");
  
  auto dtmFormat = builder.build(dtmLexems);
  FormatRegistry::connect("dtm", dtmFormat);
  


  ValuesPrinter printer;
  return 0;
}