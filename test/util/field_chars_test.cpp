#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/util/string_utils.hpp>


#define Suite FieldCharsTest

TEST(Suite, returns_false_when_field_valid) {
  EXPECT_FALSE(wellnmea::util::hasInvalidFieldChars("T"));
  EXPECT_FALSE(wellnmea::util::hasInvalidFieldChars("127.0"));
  EXPECT_FALSE(wellnmea::util::hasInvalidFieldChars("-"));
}

TEST(Suite, returns_true_when_not_valid) {
  EXPECT_TRUE(wellnmea::util::hasInvalidFieldChars("*31"));
  EXPECT_TRUE(wellnmea::util::hasInvalidFieldChars("3,1"));
}