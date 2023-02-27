#include <gtest/gtest.h>
#include <wellnmea/sentence.hpp>

using wellnmea::sentence;

TEST(SentenceChecksum, CalculatesRightChecksum) {
  sentence s("PFEC,GPint,RMC05");
  uint8_t validChecksum = 0x2d;

  EXPECT_EQ(s.checksum(), validChecksum);
}