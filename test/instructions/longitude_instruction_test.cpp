#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/token.hpp>
#include <wellnmea/formats/longitude_instruction.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/values/longitude.hpp>

#define Suite LongitudeInstructionTest

using wellnmea::Token;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, CanBeInstantiatedWithoutExceptions)
{
  LongitudeInstruction instr("name");
}

TEST(Suite, CorrectlyStoresName)
{
  LongitudeInstruction instr("name");
  EXPECT_EQ(instr.name(), "name");
}

TEST(Suite, CanBeCorrecylyCloned)
{
  LongitudeInstruction *instr = new LongitudeInstruction("name");

  auto n_instr = instr->clone("other");

  EXPECT_EQ(n_instr->name(), "other");
  EXPECT_NE(n_instr, instr) << "Clone method should return new allocated object";

  delete instr;
}

TEST(Suite, MovesIteratorForwardWhenExtractCalled)
{
  const std::string source = "$TERMB,4917.24,W";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
auto end = tokens.end();
  pos++;

  LongitudeInstruction instr("name");
  instr.extract(pos,end);
  EXPECT_EQ(pos, tokens.end());
}

TEST(Suite, ReturnsLongitudeParamWhenExtracted)
{
  const std::string source = "$TERMB,12309.57,W";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
auto end = tokens.end();
  pos++;

  LongitudeInstruction instr("name");
  auto value = instr.extract(pos,end)->as<_LongitudeValue>();

  EXPECT_EQ(value->name(), "name");
  EXPECT_THAT(value->as<LongitudeValue>(), ::testing::NotNull());

  EXPECT_EQ(value->position(), 12309.57);
  EXPECT_EQ(value->direction(), LongitudeValue::West);
}

TEST(Suite, DoReturnsNullValueOnEmptyField)
{
  const std::string source = "$TERMB,,W";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
auto end = tokens.end();
  pos++;

  LongitudeInstruction instr("name");
  auto value = instr.extract(pos,end)->as<_LongitudeValue>();

  EXPECT_EQ(value->name(), "name");
  EXPECT_THAT(value->as<NullLongitudeValue>(), ::testing::NotNull());

  EXPECT_EQ(value->direction(), std::nullopt);
  EXPECT_EQ(value->position(), std::nullopt);
}
