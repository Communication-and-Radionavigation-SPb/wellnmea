#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/util/string_utils.hpp>

#define Suite FieldHasSomeTests

TEST(Suite, returns_true_for_string_with_alphabetical_chars) {
  EXPECT_TRUE(wellnmea::util::hasNonAlpha("123"));
  EXPECT_TRUE(wellnmea::util::hasNonAlpha("T1"));
}

TEST(Suite, returns_false_for_string_without_alphabetical_chars) {
  EXPECT_FALSE(wellnmea::util::hasNonAlpha("T"));
  EXPECT_FALSE(wellnmea::util::hasNonAlpha("YQ"));
}

TEST(Suite, returns_false_for_empty_string) {
  EXPECT_FALSE(wellnmea::util::hasNonAlpha(""));
}