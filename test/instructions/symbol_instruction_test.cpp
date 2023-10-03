#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/formats/symbol_instruction.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/values/letter.hpp>

#define Suite TestLetterInstruction

using wellnmea::Token;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, can_be_managed)
{
  auto instr = new SymbolInstruction<NullLetterValue, LetterValue>("heading");
  delete instr;
}

TEST(Suite, stores_name)
{
  auto instr = SymbolInstruction<NullLetterValue, LetterValue>("heading");
  EXPECT_EQ(instr.name(), "heading");
}

TEST(Suite, can_be_cloned_correctly)
{
  SymbolInstruction<NullLetterValue, LetterValue> *instr = new SymbolInstruction<NullLetterValue, LetterValue>("heading");

  auto clone = instr->clone("clone");

  ASSERT_THAT(clone, ::testing::NotNull());
  EXPECT_EQ(clone->name(), "clone");

  delete instr;
  delete clone;
}

TEST(Suite, throws_on_invalid_symbol_met)
{
  const std::string source = "$TSTST,T";

  wellnmea::Nmea0183Lexing lex;

  auto tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  ++pos;

  SymbolInstruction<NullLetterValue, LetterValue> instr("heading", {'M', 'R'});

  EXPECT_THROW({
    instr.extract(pos, end);
  },
               wellnmea::extraction_error);
}

TEST(Suite, returns_symbol_value)
{
  const std::string source = "$TSTST,T";

  wellnmea::Nmea0183Lexing lex;

  auto tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  ++pos;

  SymbolInstruction<NullLetterValue, LetterValue> instr("heading", {'T'});

  auto value = instr.extract(pos, end)->as<_LetterValue>();

  ASSERT_THAT(value, ::testing::NotNull());

  EXPECT_EQ(value->name(), "heading");
  ASSERT_TRUE(value->value().has_value()) << "Char not extracted";

  EXPECT_EQ(value->value(), 'T');

  delete value;
}
