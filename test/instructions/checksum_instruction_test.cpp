#include <gtest/gtest.h>

#include <wellnmea/token.hpp>
#include <wellnmea/util/string_utils.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/formats/checksum_instruction.hpp>

#define Suite ChecksumTest

using namespace wellnmea;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, CanBeInstantiatedWihtoutExceptions)
{
  EXPECT_NO_THROW({
    ChecksumInstruction instr("");
  });
}

TEST(Suite, CanBeCorrectlyCloned)
{
  ChecksumInstruction *instr = new ChecksumInstruction("name");

  auto n_instr = instr->clone("other");

  EXPECT_NE(n_instr, instr);
  EXPECT_NE(n_instr, nullptr);
}

TEST(Suite, MovesIteratorForwardWhenExtractionExecuted)
{
  const std::string source = "$TEHDT*2D";

  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  ChecksumInstruction instr("name");
  instr.extract(pos, end);
  EXPECT_EQ(pos, tokens.end());
}

TEST(Suite, ExtractsChecksumAndCastsItCorrectly)
{
  const std::string source = "$TEHDT*2D";

  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  ChecksumInstruction instr("name");
  auto value = instr.extract(pos, end);

  EXPECT_NE(value->as<ChecksumValue>(), nullptr);
  EXPECT_EQ(value->as<ChecksumValue>()->name(), "name");
  EXPECT_EQ(value->as<ChecksumValue>()->value(), 45);
}

TEST(Suite, ReturnsNullChecksumValueWhenNotProvided)
{
  const std::string source = "$TEHDT";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);
  auto pos = tokens.begin();
  auto end = tokens.end();
  pos++;

  ChecksumInstruction instr("name");
  auto value = instr.extract(pos, end);

  EXPECT_NE(value->as<NullChecksumValue>(), nullptr);
  EXPECT_EQ(value->as<NullChecksumValue>()->value(), std::nullopt);
}

TEST(Suite, ThrowsExceptionWhenRequiredChecksumNotProvided)
{
  const std::string source = "$TEHDT";
  wellnmea::Nmea0183Lexing lex;

  std::list<Token> tokens = lex.splitTokens(source);

  auto pos = tokens.begin();
  auto end = tokens.end();

  pos++;

  ChecksumInstruction instr("name", true);

  EXPECT_THROW({
    try
    {
      auto value = instr.extract(pos, end);
    }
    catch (const std::exception &e)
    {
      EXPECT_EQ(std::string(e.what()), "Checksum marked as required but provided message do not"
                                       " contains that field.");
      throw;
    }
  },
               extraction_error);
}