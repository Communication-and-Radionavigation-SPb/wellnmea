#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.hpp"

#include <wellnmea/instructions/number_instruction.hpp>
#include <wellnmea/sentence.hpp>

#define Suite NumberInstructionTests

TEST(Suite, can_be_instantiated) {
  EXPECT_NO_THROW({ wellnmea::instructions::NumberInstruction instr(""); });
}

TEST(Suite, can_be_cloned) {
  test_clonable<wellnmea::instructions::NumberInstruction>();
}

TEST(Suite, which_expected_to_be_correct) {
  wellnmea::instructions::NumberInstruction instr("");

  EXPECT_EQ(instr.which(), "number");
}

TEST(Suite, moves_iterator_forward) {
  wellnmea::instructions::NumberInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr.extract(it, end);
  EXPECT_EQ(it, end);
}

TEST(Suite, returns_non_null_value) {
  wellnmea::instructions::NumberInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);

  EXPECT_NE(value, nullptr);
}

TEST(Suite, returns_empty_value_when_field_empty) {
  wellnmea::instructions::NumberInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr.extract(it, end)->as<wellnmea::instructions::NumberValue>();

  ASSERT_FALSE(value->getValue().has_value());
  EXPECT_EQ(value->getValue(), std::nullopt);
}

TEST(Suite, returns_correct_value) {
  wellnmea::instructions::NumberInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("98.035");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr.extract(it, end)->as<wellnmea::instructions::NumberValue>();

  ASSERT_TRUE(value->getValue().has_value());
  EXPECT_THAT(value->getValue().value(), ::testing::DoubleNear(98.035, 0.001));
}

TEST(Suite, understands_negative_field) {
  wellnmea::instructions::NumberInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("-98.035");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr.extract(it, end)->as<wellnmea::instructions::NumberValue>();

  ASSERT_TRUE(value->getValue().has_value());
  EXPECT_THAT(value->getValue().value(), ::testing::DoubleNear(-98.035, 0.001));
}

TEST(Suite, range_number_throws_when_value_not_matches) {
  wellnmea::instructions::RangeNumberInstruction instr("",
                                                      std::make_tuple(0, 4));

  wellnmea::Sentence sentence;

  sentence.fields.push_back("5");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  EXPECT_THROW({ instr.extract(it, end); }, wellnmea::NumberRangeError);
}
