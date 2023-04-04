#include <gtest/gtest.h>

#include <wellnmea/formats/utc.hpp>

#define Suite UTCFormatTest

TEST(Suite, InstantiatesWithoutExceptions)
{
  wellnmea::formats::UTC fmt;
}