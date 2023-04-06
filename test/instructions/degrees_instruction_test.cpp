#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define Suite DegreesInstructionTest

#include <wellnmea/token.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/formats/degreese_instruction.hpp>
#include <wellnmea/values/degrees.hpp>

using wellnmea::Token;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, CanBeInstantiatedWithoutExceptions)
{
  DegreesInstruction instr("name");
}

TEST(Suite, CorrectlyStoresName)
{
  DegreesInstruction instr("name");
  EXPECT_EQ(instr.name(), "name");
}

TEST(Suite, CanBeCorrectlyCloned)
{
  DegreesInstruction *instr = new DegreesInstruction("name");

  auto n_instr = instr->clone("other");

  EXPECT_EQ(n_instr->name(), "other");
  EXPECT_NE(n_instr, instr) << "Clone method should return new allocated object";

  delete instr;
  delete n_instr;
}

TEST(Suite, MovesIteratorForwardWhenExtractCalled)
{
  const std::string source = "$TERMB,051.9,T";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  DegreesInstruction instr("name");

  instr.extract(pos);

  EXPECT_EQ(pos, tokens.end());
}

TEST(Suite, ReturnsDegreesParamWhenExtracted)
{
  const std::string source = "$TERMB,051.9,T";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  DegreesInstruction instr("name");
  auto value = instr.extract(pos)->as<DegreesValue>();

  EXPECT_EQ(value->name(), "name");
  EXPECT_THAT(value, ::testing::A<DegreesValue*>());

  EXPECT_EQ(value->cursor(), 51.9);
  EXPECT_EQ(value->measure(), DegreesValue::True);
}

TEST(Suite, CanRecognizeMagneticMeasure) {
  const std::string source = "$TERMB,051.9,M";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  DegreesInstruction instr("name");
  auto value = instr.extract(pos)->as<_DegreesValue>();

  EXPECT_EQ(value->cursor(), 51.9);
  EXPECT_EQ(value->measure(), DegreesValue::Magnetic);
}

TEST(Suite, DoReturnsNullValueWhenDigitalTokenIsEmpty) {
  const std::string source = "$TERMB,,M";
  wellnmea::Nmea0183Lexing lex;

  auto tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  DegreesInstruction instr("name");
  auto value = instr.extract(pos)->as<_DegreesValue>();

  EXPECT_EQ(value->cursor(), std::nullopt);
  EXPECT_EQ(value->measure(), std::nullopt);
}

TEST(Suite, DoReturnsNullValueWhenMeasureTokenIsEmtpy) {
  const std::string source = "$TERMB,51.0,";
  wellnmea::Nmea0183Lexing lex;

  auto tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  DegreesInstruction instr("name");
  auto value = instr.extract(pos)->as<_DegreesValue>();
  
  EXPECT_EQ(value->cursor(), std::nullopt);
  EXPECT_EQ(value->measure(), std::nullopt);
}