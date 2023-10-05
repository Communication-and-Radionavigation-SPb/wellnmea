#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/util/number_utils.hpp>

#define Suite NumberUtilsTests

TEST(Suite, to_double_returns_double_for_valid_number_string)
{
  double value = wellnmea::util::toDouble("26.0");
  EXPECT_EQ(value, 26.0);
}

TEST(Suite, to_int_returns_10_based_value_for_valid_number_string)
{
  int value = wellnmea::util::toInt("20");
  EXPECT_EQ(value, 20);
}

TEST(Suite, to_int_returns_16_based_value_for_valid_number_string)
{
  int64_t value = wellnmea::util::toInt("ff", 16);
  EXPECT_EQ(value, 0xff);
}

TEST(Suite, to_double_throws_on_invalid_string_argument)
{
  EXPECT_THROW({
    wellnmea::util::toDouble("25.0t");
  },
               wellnmea::NumberDecodeError);
}