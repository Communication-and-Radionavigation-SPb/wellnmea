#include <gtest/gtest.h>

#include <wellnmea/token.hpp>
#include <wellnmea/nmea0183_lexing.hpp>

#define Suite MessageInterpreterTest

TEST(Suite, CorrectlyDetectsPhrases)
{
  wellnmea::Token::Type type;
  wellnmea::Nmea0183Lexing interpreter;

  std::list<std::string> phrases{"W84", "w84", "5A"};

  for (auto &&slice : phrases)
  {
    type = interpreter.detectTokenType(slice);
    EXPECT_EQ(type, wellnmea::Token::phrase) << slice << " have invalid type detected";
  }
}

TEST(Suite, CorrectlyDetectsNumbers)
{
  wellnmea::Token::Type type;
  wellnmea::Nmea0183Lexing interpreter;

  std::list<std::string> numbers{"0.1", "-12", "02841.825", "6109.876"};

  for (auto &&slice : numbers)
  {
    type = interpreter.detectTokenType(slice);
    EXPECT_EQ(type, wellnmea::Token::number) << slice << " have invalid type detected";
  }
}

TEST(Suite, CorrectlyDetectsSymbols)
{
  wellnmea::Token::Type type;

  wellnmea::Nmea0183Lexing interpreter;

  std::list<std::string> symbols{"A", "M", "N", "S", "W", "E",
                                 "a", "m", "n", "s", "w", "e"};

  for (auto &&slice : symbols)
  {
    type = interpreter.detectTokenType(slice);
    EXPECT_EQ(type, wellnmea::Token::symbol) << slice << " have invalid type detected";
  }
}

TEST(Suite, CorrectlyDetectsChecksums)
{
  wellnmea::Token::Type type;

  wellnmea::Nmea0183Lexing interpreter;

  std::list<std::string> checksums{"*35", "*0F", "*34"};

  for (auto &&slice : checksums)
  {
    type = interpreter.detectTokenType(slice);
    EXPECT_EQ(type, wellnmea::Token::checksum) << slice << " have invalid type detected";
  }
}

TEST(Suite, CorrectlyDetectsNullTokens)
{
  wellnmea::Token::Type type;

  wellnmea::Nmea0183Lexing interpreter;

  type = interpreter.detectTokenType("");
  EXPECT_EQ(type, wellnmea::Token::null);
}

TEST(Suite, CorrectlyDetectsInvalidTokens)
{

  wellnmea::Nmea0183Lexing interpreter;

  std::list<std::string> invalid_messages{
      ".23",
  };

  for (auto &&slice : invalid_messages)
  {
    EXPECT_THROW({
      interpreter.detectTokenType(slice);
    },
                 wellnmea::parse_error);
  }
}

TEST(Suite, ThrowsOn$Apsent)
{

  wellnmea::Nmea0183Lexing interpreter;

  EXPECT_THROW({
    interpreter.splitTokens("TEDTM,W84,,,,,,,*17");
  },
               wellnmea::parse_error);
}

TEST(Suite, DoNotThrowsOnValidMessage)
{

  wellnmea::Nmea0183Lexing interpreter;

  EXPECT_NO_THROW({ interpreter.splitTokens("$TEDTM,W84,,,,,,,*17"); });
}

TEST(Suite, ReturnsCorrectListOfTokens)
{
  wellnmea::Nmea0183Lexing interpreter;

  auto tokens = interpreter.splitTokens("$TEDTM,W84,,,,,,,*17");
  
}