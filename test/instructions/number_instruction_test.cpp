#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/token.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/formats/number_instruction.hpp>
#include <wellnmea/values/number.hpp>

#define Suite NumberInstructionTest

using wellnmea::Token;
using namespace wellnmea::formats;
using namespace wellnmea::values;

class TestNumberInstruction : public NumberInstruction
{
public:
  TestNumberInstruction() : NumberInstruction("test") {}
};

TEST(Suite, CanBeInstantiated)
{
  TestNumberInstruction instr;
}

TEST(Suite, CanBeClonseCorrectly)
{
  TestNumberInstruction *instr = new TestNumberInstruction();

  auto n_instr = instr->clone("name");

  EXPECT_NE(n_instr, instr) << "Clone method should return new allocated object";
  delete n_instr;
  delete instr;
}

TEST(Suite, MovesInteratorForwardWhenExtractCalled)
{
  const std::string source = "$TSTST,4917.24";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  TestNumberInstruction instr;
  instr.extract(pos, end);
  EXPECT_EQ(pos, tokens.end());
}

TEST(Suite, ReturnsNumberValueWhenExtracted)
{
  const std::string source = "$TSTST,49.2";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  TestNumberInstruction instr;
  auto value = instr.extract(pos, end)->as<_NumberValue>();

  EXPECT_EQ(value->name(), "test");
  EXPECT_THAT(value->as<NumberValue>(), ::testing::NotNull());

  EXPECT_THAT(value->value(), 49.2);

  delete value;
}

TEST(Suite, DoReturnsNullValueOnEmptyField)
{
  const std::string source = "$TSTST,";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  TestNumberInstruction instr;
  auto value = instr.extract(pos, end)->as<_NumberValue>();

  EXPECT_EQ(value->name(), "test");
  EXPECT_THAT(value->as<NullNumberValue>(), ::testing::NotNull());

  EXPECT_EQ(value->value(), std::nullopt);
}