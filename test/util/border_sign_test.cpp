#include <gtest/gtest.h>

#define Suite BorderSignTest

#include <wellnmea/util/string_utils.hpp>

using namespace wellnmea::util;

TEST(Suite, CorrectlyDetectsBorderByProvidedSign) {
  std::string source = "name[time|utc;long|longitude]";
  std::string source2 = "name[time|utc;long|longitude];";

  auto pos = border_sign(';', {{'[',']'}}, source.begin(), source.end());
  auto pos2 = border_sign(';', {{'[',']'}}, source2.begin(), source2.end());
  EXPECT_EQ(pos, source.end()) << "Invalid position found: " << pos.base();
  EXPECT_EQ(pos2, source2.end() - 1) << "Invalid position found: " << pos2.base();
}