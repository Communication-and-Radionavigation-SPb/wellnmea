#include <gtest/gtest.h>

#include <string>
#include <list>
#include <wellnmea/token.hpp>

#define Suite TokenTest

TEST(Suite, InstantiatesWithoutExceptions)
{
  wellnmea::Token token(wellnmea::Token::Type::number, "123");
}

TEST(Suite, CorrectlyStoresDataOnConstruction)
{
  wellnmea::Token token(wellnmea::Token::number, "123");
  EXPECT_EQ(token.type, wellnmea::Token::number);
  EXPECT_EQ(token.slice, "123");
}