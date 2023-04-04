#include <gtest/gtest.h>

#include <string>
#include <wellnmea/util/string_utils.hpp>

#define Suite UtilExtractBetween

TEST(Suite, CorrectlyExtractsEnclosedContent)
{
  const std::string enclosure = "[[some]]";
  auto pos = wellnmea::util::extract_between('[', ']', enclosure.begin(), enclosure.end());

  auto enclosed = std::string(enclosure.begin() + 1, pos);

  EXPECT_EQ(enclosed, "[some]");
}

TEST(Suite, WorksWithOtherChar) {
  const std::string enclosure = "((some))";
  auto pos = wellnmea::util::extract_between('(', ')', enclosure.begin(), enclosure.end())  ;

  auto enclosed = std::string(enclosure.begin() + 1, pos);

  EXPECT_EQ(enclosed, "(some)");
}

TEST(Suite, WorksWithExtraBracket) {
  const std::string enclosure = "((some)))";

  auto pos = wellnmea::util::extract_between('(', ')', enclosure.begin(), enclosure.end())  ;

  auto enclosed = std::string(enclosure.begin() + 1, pos);

  EXPECT_EQ(enclosed, "(some)");
}