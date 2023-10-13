#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string_view>
#include <wellnmea/util/number_utils.hpp>

#define Suite NumberUtilsTests

TEST(Suite, to_double_returns_double_for_valid_number_string)
{
  std::string param = "26.0";
  double value = wellnmea::util::toDouble(param);
  EXPECT_EQ(value, 26.0);
}

TEST(Suite, to_double_recognises_decimal_part)
{
  std::string param = "123012.623";
  double value = wellnmea::util::toDouble(param);
  EXPECT_EQ(value, 123012.623);
}

TEST(Suite, to_double_works_with_string_view)
{
  std::string s_param = "123012.623";
  std::string_view param = std::string_view(s_param.c_str(), s_param.size());

  double value = wellnmea::util::toDouble(param);
  EXPECT_EQ(value, 123012.623);
}

TEST(Suite, to_double_understands_leading_zeros) {
  std::string param = "003000.623";
  double value = wellnmea::util::toDouble(param);
  EXPECT_EQ(value, 3000.623);
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
    std::string param = "25.0t";
    wellnmea::util::toDouble(param);
  },
               wellnmea::NumberDecodeError);
}