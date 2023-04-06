#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/token.hpp>
#include <wellnmea/formats/repeated_instruction.hpp>
#include <wellnmea/formats/degreese_instruction.hpp>
#include <wellnmea/nmea0183_lexing.hpp>

#define Suite RepeatedInstructionTest

using wellnmea::Token;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, CanBeInstantiatedWithoutExceptions)
{
  auto instr = RepeatedInstruction("name", {});
}

TEST(Suite, CorrectlyStoresName)
{
  auto instr = RepeatedInstruction("name", {});

  EXPECT_EQ(instr.name(), "name");
}

TEST(Suite, MovesIteratorForwardWhenExtractCalled)
{
  auto subfield = std::make_shared<DegreesInstruction>("degrees");
  auto instr = RepeatedInstruction("directions", {subfield});

  const std::string source = "$TERMB,4917.24,N,4917.24,N";
  const std::string checksumsource = "$TERMB,4917.24,N,4917.24,N*45";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  std::list<Token> tokens_with_check = lex.splitTokens(checksumsource);
  auto pos = tokens.begin();
  auto end = tokens.end();
  auto chpos = tokens_with_check.begin();
  auto chend = tokens_with_check.end();
  pos++;
  chpos++;

  instr.extract(pos, end);
  EXPECT_EQ(pos, tokens.end());

  instr.extract(chpos, chend);
  EXPECT_EQ(chpos, --tokens_with_check.end());
}

TEST(Suite, ReturnsRepeatedParamsWhenExtracted)
{

  auto subfield = std::make_shared<DegreesInstruction>("degrees");
  auto instr = RepeatedInstruction("directions", {subfield});

  const std::string source = "$TERMB,4917.24,T,5917.24,M,,";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  auto value = instr.extract(pos, end)->as<RepeatedValue>();

  EXPECT_THAT(value, ::testing::NotNull());
  EXPECT_EQ(value->size(), 3);

  std::vector<std::optional<double>> valuesC{4917.24, 5917.24, std::nullopt};
  std::vector<std::optional<DegreesValue::Measure>> valuesM{
      DegreesValue::Measure::True,
      DegreesValue::Measure::Magnetic,
      std::nullopt};

  auto itc = valuesC.begin();
  auto itm = valuesM.begin();

  for (auto it = value->begin(); it != value->end(); it++, itc++, itm++)
  {
    auto subv = it->as<_DegreesValue>();
    auto cursor = subv->cursor();
    auto measure = subv->measure();
    EXPECT_EQ(measure, *itm);
    EXPECT_EQ(cursor, *itc);
  }
}