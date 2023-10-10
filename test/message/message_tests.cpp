#include <gtest/gtest.h>

#include <wellnmea/message.hpp>

#define Suite MessageTests

namespace wn
{
  using namespace wellnmea;
}

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wn::Message message("", "", {});
  });
}

TEST(Suite, correctly_stores_talker) {
  wn::Message message("talker", "", {});
  EXPECT_EQ(message.talker(), "talker");
}

TEST(Suite, correctly_stores_formatter) {
  wn::Message message("", "formatter", {});
  EXPECT_EQ(message.formatter(), "formatter");
}