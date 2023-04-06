#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/token.hpp>
#include <wellnmea/formats/latitude_instruction.hpp>
#include <wellnmea/values/latitude.hpp>
#include <wellnmea/nmea0183_lexing.hpp>

#define Suite LatitudeInstructionTest

using wellnmea::Token;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, CanBeInstantiatedWithoutExceptions)
{
  LatitudeInstruction instr("name");
}

TEST(Suite, CorrectlyStoresName)
{
  LatitudeInstruction instr("name");
  EXPECT_EQ(instr.name(), "name");
}

TEST(Suite, CanBeCorrecylyCloned)
{
  LatitudeInstruction *instr = new LatitudeInstruction("name");

  auto n_instr = instr->clone("other");

  EXPECT_EQ(n_instr->name(), "other");
  EXPECT_NE(n_instr, instr) << "Clone method should return new allocated object";

  delete instr;
}

TEST(Suite, MovesIteratorForwardWhenExtractCalled)
{
  const std::string source = "$TERMB,4917.24,N";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  LatitudeInstruction instr("name");
  instr.extract(pos);
  EXPECT_EQ(pos, tokens.end());
}
TEST(Suite, ReturnsLongitudeParamWhenExtracted)
{
  const std::string source = "$TERMB,4917.24,N";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  LatitudeInstruction instr("name");
  auto value = instr.extract(pos)->as<LatitudeValue>();

  EXPECT_EQ(value->name(), "name");
  EXPECT_THAT(value, ::testing::A<LatitudeValue *>());

  EXPECT_EQ(value->position(), 4917.24);
  EXPECT_EQ(value->direction(), LatitudeValue::North);
}

TEST(Suite, CanRecognizeSouthDirection)
{
  const std::string source = "$TERMB,4917.24,S";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  LatitudeInstruction instr("name");
  auto value = instr.extract(pos)->as<LatitudeValue>();

  EXPECT_EQ(value->name(), "name");
  EXPECT_THAT(value, ::testing::A<LatitudeValue *>());

  EXPECT_EQ(value->position(), 4917.24);
  EXPECT_EQ(value->direction(), LatitudeValue::South);
}

TEST(Suite, DoNotFailsOnInvalidDirectionSymbol)
{
  const std::string source = "$TERMB,4917.24,u";
  //                              invalid <-|
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  LatitudeInstruction instr("name");
  auto value = instr.extract(pos)->as<LatitudeValue>();

  EXPECT_EQ(value->name(), "name");
  EXPECT_THAT(value, ::testing::A<LatitudeValue *>());

  EXPECT_EQ(value->position(), 4917.24);
  EXPECT_EQ(value->direction(), LatitudeValue::Unknown);
}

TEST(Suite, DoReturnsNullValueWhenTokenIsEmpty)
{
  const std::string source = "$TERMB,,S";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  LatitudeInstruction instr("name");
  auto value = instr.extract(pos)->as<_LatitudeValue>();

  EXPECT_THAT(value->as<NullLatitudeValue>(), ::testing::NotNull());
  EXPECT_EQ(value->as<NullLatitudeValue>()->direction(), std::nullopt);
  EXPECT_EQ(value->as<NullLatitudeValue>()->position(), std::nullopt);
  EXPECT_EQ(value->as<NullLatitudeValue>()->serialise(), std::nullopt);
}