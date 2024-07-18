#include <gtest/gtest.h>

#include <string>
#include <wellnmea/util/string_utils.hpp>

TEST(UtilExtractBetween, CorrectlyExtractsEnclosedContent) {
  const std::string enclosure = "[[some]]";
  auto pos = wellnmea::util::extract_between('[', ']', enclosure.begin(),
                                             enclosure.end());

  auto enclosed = std::string(enclosure.begin() + 1, pos);

  EXPECT_EQ(enclosed, "[some]");
}

TEST(UtilExtractBetween, WorksWithOtherChar) {
  const std::string enclosure = "((some))";
  auto pos = wellnmea::util::extract_between('(', ')', enclosure.begin(),
                                             enclosure.end());

  auto enclosed = std::string(enclosure.begin() + 1, pos);

  EXPECT_EQ(enclosed, "(some)");
}

TEST(UtilExtractBetween, WorksWithExtraBracket) {
  const std::string enclosure = "((some)))";

  auto pos = wellnmea::util::extract_between('(', ')', enclosure.begin(),
                                             enclosure.end());

  auto enclosed = std::string(enclosure.begin() + 1, pos);

  EXPECT_EQ(enclosed, "(some)");
}
