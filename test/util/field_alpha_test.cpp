#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/util/string_utils.hpp>

TEST(FieldHasSomeTests, returns_true_for_string_with_alphabetical_chars) {
  EXPECT_TRUE(wellnmea::util::hasNonAlpha("123"));
  EXPECT_TRUE(wellnmea::util::hasNonAlpha("T1"));
}

TEST(FieldHasSomeTests, returns_false_for_string_without_alphabetical_chars) {
  EXPECT_FALSE(wellnmea::util::hasNonAlpha("T"));
  EXPECT_FALSE(wellnmea::util::hasNonAlpha("YQ"));
}

TEST(FieldHasSomeTests, returns_false_for_empty_string) {
  EXPECT_FALSE(wellnmea::util::hasNonAlpha(""));
}
