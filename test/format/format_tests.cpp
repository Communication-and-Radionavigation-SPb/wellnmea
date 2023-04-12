#include <gtest/gtest.h>

#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/formats/degreese_instruction.hpp>
#include <wellnmea/formats/longitude_instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>
#include <wellnmea/default_format_builder.hpp>
#include <wellnmea/formats/format_interpreter.hpp>
#include <wellnmea/nmea0183_lexing.hpp>

#define Suite FormatTest

using namespace wellnmea::formats;
using namespace wellnmea::values;
using wellnmea::Nmea0183Lexing;

TEST(Suite, FormatCanParseInARightWay)
{
  auto dgrs = std::make_shared<DegreesInstruction>("");
  auto lng = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(dgrs);
  InstructionsRegistry::add(lng);

  Nmea0183Lexing tokenizer;
  auto tokens = tokenizer.splitTokens("$TEHDT,49.6,M,566,W");

  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("block[direction|degrees;position|longitude;];");

    auto fmt = builder.build(lexems);
  auto begin = ++tokens.begin();

  auto result = fmt->parse(begin, tokens.end()); // Skipped talker and formatter token

  EXPECT_EQ(result.size(), 1);
}