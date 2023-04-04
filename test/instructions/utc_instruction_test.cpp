#include <gtest/gtest.h>

#include <wellnmea/token.hpp>
#include <wellnmea/formats/utc_instruction.hpp>
#include <wellnmea/nmea0183_lexing.hpp>

#define Suite UtcInstructionTest

using wellnmea::Token;
using namespace wellnmea::formats;

TEST(Suite, CanBeInstantiatedWithoutExceptions)
{
  UTCInstruction utc("name");
}

TEST(Suite, CorrectlyStoresName) {
  UTCInstruction instr("name");
  EXPECT_EQ(instr.name(), "name");
}

TEST(Suite, CanBeCorrecylyCloned) {
  UTCInstruction* instr = new UTCInstruction("name");

  auto n_instr = instr->clone("other");

  EXPECT_EQ(n_instr->name(), "other");
  EXPECT_NE(n_instr, instr) << "Clone method should return new allocated object";

  delete instr;
}

TEST(Suite, MovesIteratorForwardWhenExtractCalled) {
  const std::string source = "$TERMC,240000";
  wellnmea::Nmea0183Lexing lex;



  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  UTCInstruction instr("name");
  instr.extract(pos);
  EXPECT_EQ(pos, tokens.end());
}

TEST(Suite, ReturnsUtcParamWhenExtracted) {
  const std::string source = "$TERMC,240000";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  pos++;

  UTCInstruction instr("name");
  auto value = instr.extract(pos);

  // TODO: Check that utc value returned
  // TODO: Check that utc value has correct props
}
