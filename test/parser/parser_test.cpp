#include <gtest/gtest.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/formats/formats_registry.hpp>

#define Suite Parser

TEST(Suite, InstantiatesWithoutExceptions)
{
  EXPECT_NO_THROW({
    auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();
    wellnmea::Parser parser(lex, {});
  });
}

TEST(Suite, ReturnsCorrectParsedMessage)
{
  auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();
  wellnmea::Parser parser(lex, {});

  wellnmea::NmeaMessage msg = parser.parse("$TEDTM,W84,,,,,,,*17");

  EXPECT_EQ(msg.talker, "TE");
  EXPECT_EQ(msg.formatter, "DTM");
}
