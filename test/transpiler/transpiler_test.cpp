#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/transpiler.hpp>
#include <wellnmea/formats/format.hpp>

#include "mocked_format.hpp"

#define Suite TranspilerTest

TEST(Suite, can_be_intantiated)
{
  EXPECT_NO_THROW({
    wellnmea::Transpiler transpiler;
  });
}

TEST(Suite, can_register_format)
{
  wellnmea::Transpiler transpiler;

  wellnmea::Transpiler::FmtPtr fmt = wellnmea::Transpiler::FmtPtr(new MockedFormat());

  transpiler.connect("som", fmt);

  EXPECT_TRUE(transpiler.contains("som"));
}

TEST(Suite, understands_both_uppercase_and_lowercase_formatters)
{
  wellnmea::Transpiler transpiler;
  auto fmt_ptr = new MockedFormat();
  auto fmt = wellnmea::Transpiler::FmtPtr(fmt_ptr);
  transpiler.connect("som", fmt);
  std::list<wellnmea::formats::Format::ValuePtr> l{};

  EXPECT_CALL(*fmt_ptr, parse(::testing::_))
      .Times(2)
      .WillRepeatedly(::testing::Return(l));

  wellnmea::Sentence sentence;

  sentence.formatter = "som";
  transpiler.parse(sentence);

  sentence.formatter = "SOM";
  transpiler.parse(sentence);
}

TEST(Suite, returns_message_pointer)
{
  wellnmea::Transpiler transpiler;
  wellnmea::Transpiler::FmtPtr fmt = wellnmea::Transpiler::FmtPtr(new MockedFormat());
  transpiler.connect("som", fmt);

  wellnmea::Sentence sentence;
  sentence.formatter = "som";

  auto msg = transpiler.parse(sentence);

  EXPECT_THAT(msg, ::testing::Not(::testing::Eq(nullptr)));
}

TEST(Suite, returns_message_with_correct_values)
{
  wellnmea::Transpiler transpiler;
  auto fmt_ptr = new MockedFormat();
  auto fmt = wellnmea::Transpiler::FmtPtr(fmt_ptr);
  transpiler.connect("som", fmt);

  wellnmea::Sentence sentence;
  sentence.formatter = "som";

  auto fake = std::make_shared<FakeValue>();
  std::list<wellnmea::formats::Format::ValuePtr> l{fake};
  EXPECT_CALL(*fmt_ptr, parse(::testing::_))
      .WillRepeatedly(::testing::Return(l));

  auto msg = transpiler.parse(sentence);

  EXPECT_EQ(msg->values().size(), 1);
  EXPECT_EQ(msg->values().begin()->get()->name(), "fake");
}