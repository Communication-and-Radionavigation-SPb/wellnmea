#include <gtest/gtest.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/formats_registry.hpp>

#define Suite Parser

TEST(Suite, InstantiatesWithoutExceptions)
{
  EXPECT_NO_THROW({
    wellnmea::Parser parser({});
  });
}


TEST(Suite, ReturnsCorrectParsedMessage)
{
  wellnmea::Parser parser({});

  wellnmea::NmeaMessage msg = parser.parse("$TEDTM,W84,,,,,,,*17");

  EXPECT_EQ(msg.talker, "TE");
  EXPECT_EQ(msg.formatter, "DTM");
}
