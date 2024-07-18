#include <gtest/gtest.h>

#include <wellnmea/message.hpp>

namespace wn {
using namespace wellnmea;
}

TEST(MessageTests, can_be_instantiated) {
  EXPECT_NO_THROW({ wn::Message message("", "", {}); });
}

TEST(MessageTests, correctly_stores_talker) {
  wn::Message message("talker", "", {});
  EXPECT_EQ(message.talker(), "talker");
}

TEST(MessageTests, correctly_stores_formatter) {
  wn::Message message("", "formatter", {});
  EXPECT_EQ(message.formatter(), "formatter");
}
