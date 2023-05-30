#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/message.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/formats/degreese_instruction.hpp>
#include <wellnmea/formats/longitude_instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>
#include <wellnmea/formats/format_interpreter.hpp>
#include <wellnmea/default_format_builder.hpp>
#include <wellnmea/formats/formats_registry.hpp>

#define Suite Parser

using wellnmea::Message;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, InstantiatesWithoutExceptions)
{
  EXPECT_NO_THROW({
    auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();
    wellnmea::Parser parser(lex, {});
  });
}

TEST(Suite, ThrowsOnInvalidMessageFormat)
{

  auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();

  auto dgrs = std::make_shared<DegreesInstruction>("");
  auto lng = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(dgrs);
  InstructionsRegistry::add(lng);

  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("block[direction|degrees;position|longitude;];");

  auto fmt = builder.build(lexems);

  FormatRegistry::connect("DTM", fmt);

  wellnmea::Parser parser(lex, {"DTM"});

  EXPECT_THROW({ parser.parse("$TEDTM,W84,,,,,,,*17"); }, wellnmea::extraction_error);
}

TEST(Suite, ReturnsCorrectParsedMessage)
{

  FormatRegistry::clear();

  auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();

  auto dgrs = std::make_shared<DegreesInstruction>("");
  auto lng = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(dgrs);
  InstructionsRegistry::add(lng);

  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("block[direction|degrees;position|longitude;];");

  auto fmt = builder.build(lexems);

  FormatRegistry::connect("DTM", fmt);

  wellnmea::Parser parser(lex, {"DTM"});

  std::shared_ptr<Message> msg = parser.parse("$TEDTM,84,M,98.9,E,54.9,T,55.9,W*17");

  EXPECT_EQ(msg->talker(), "TE");
  EXPECT_EQ(msg->formatter(), "DTM");
  EXPECT_EQ(msg->values().size(), 1);
  auto values = msg->values();
  auto pos = values.begin()->get()->as<RepeatedValue>();
  auto it = pos->begin();
}