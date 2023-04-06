#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/token.hpp>
#include <wellnmea/formats/utc_instruction.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/values/utc.hpp>

#define Suite UtcInstructionTest

using wellnmea::Token;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, CanBeInstantiatedWithoutExceptions)
{
  UTCInstruction utc("name");
}

TEST(Suite, CorrectlyStoresName)
{
  UTCInstruction instr("name");
  EXPECT_EQ(instr.name(), "name");
}

TEST(Suite, CanBeCorrecylyCloned)
{
  UTCInstruction *instr = new UTCInstruction("name");

  auto n_instr = instr->clone("other");

  EXPECT_EQ(n_instr->name(), "other");
  EXPECT_NE(n_instr, instr) << "Clone method should return new allocated object";

  delete instr;
}

TEST(Suite, MovesIteratorForwardWhenExtractCalled)
{
  const std::string source = "$TERMC,240000";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  UTCInstruction instr("name");
  instr.extract(pos, end);
  EXPECT_EQ(pos, tokens.end());
}

TEST(Suite, ReturnsUtcParamWhenExtracted)
{
  const std::string source = "$TERMC,010203";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  UTCInstruction instr("name");
  auto value = instr.extract(pos, end)->as<_UTCValue>();

  EXPECT_EQ(value->name(), "name");

  EXPECT_THAT(value->hours(), ::testing::Eq(1));
  EXPECT_THAT(value->minutes(), ::testing::Eq(2));
  EXPECT_THAT(value->seconds(), ::testing::Eq(3));
  EXPECT_THAT(value->milliseconds(), ::testing::Eq(0));
}

TEST(Suite, TestOtherUTCTimestampExtracted)
{
  const std::string source = "$TERMC,123456.999,113000.26";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  UTCInstruction instr("name");
  auto value = instr.extract(pos, end)->as<_UTCValue>();
  auto nextvalue = instr.extract(pos, end)->as<_UTCValue>();

  EXPECT_EQ(value->name(), "name");
  EXPECT_THAT(value->as<UTCValue>(), ::testing::NotNull());
  EXPECT_THAT(value->hours(), ::testing::Eq(12));
  EXPECT_THAT(value->minutes(), ::testing::Eq(34));
  EXPECT_THAT(value->seconds(), ::testing::Eq(56));
  EXPECT_THAT(value->milliseconds(), ::testing::Eq(999));

  EXPECT_EQ(nextvalue->hours(), 11);
  EXPECT_EQ(nextvalue->minutes(), 30);
  EXPECT_EQ(nextvalue->seconds(), 0);
  EXPECT_EQ(nextvalue->milliseconds(), 260);
}