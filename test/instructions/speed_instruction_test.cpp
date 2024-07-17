#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.hpp"

#include <wellnmea/instructions/speed_instruction.hpp>
#include <wellnmea/sentence.hpp>

#define Suite SpeedInstructionTests

class SpeedInstructionTests : public testing::Test {
 public:
  wellnmea::instructions::SpeedInstruction* instr;

 protected:
  void SetUp() override {
    instr = new wellnmea::instructions::SpeedInstruction("wind");
  }

  void TearDown() override { delete instr; }
};

TEST_F(Suite, has_correct_name) {
  EXPECT_EQ(instr->name(), "wind");
}

TEST_F(Suite, can_be_cloned) {
  test_clonable<wellnmea::instructions::SpeedInstruction>();
}

TEST_F(Suite, which_expected_to_be_correct) {
  EXPECT_EQ(instr->which(), "speed");
}

TEST_F(Suite, moves_iterator_forward) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("K");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr->extract(it, end);
  EXPECT_NE(it, sentence.fields.begin());
}

TEST_F(Suite, moves_iterator_two_steps_forward) {

  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("K");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr->extract(it, end);
  EXPECT_EQ(it, sentence.fields.end());
}

TEST_F(Suite, returns_non_null_value) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("M");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr->extract(it, end);

  EXPECT_NE(value, nullptr);
}

TEST_F(Suite, value_has_correct_name) {

  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("M");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr->extract(it, end);

  EXPECT_EQ(value->name(), "wind");
}

TEST_F(Suite, returns_correct_value_for_knots) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("12.0");
  sentence.fields.push_back("N");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr->extract(it, end)->as<wellnmea::instructions::SpeedValue>();

  ASSERT_TRUE(value->raw().has_value());
  EXPECT_THAT(value->raw().value(), ::testing::DoubleNear(12.0, 0.01));
  EXPECT_THAT(value->units().value_or('x'), ::testing::Eq('N'));
  EXPECT_THAT(value->converted().value(),
              ::testing::DoubleNear(12.0 * 1.852, 0.001));
}

TEST_F(Suite, returns_correct_value_for_ms) {

  wellnmea::Sentence sentence;

  sentence.fields.push_back("12.0");
  sentence.fields.push_back("M");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr->extract(it, end)->as<wellnmea::instructions::SpeedValue>();

  ASSERT_TRUE(value->raw().has_value());
  EXPECT_THAT(value->raw().value(), ::testing::DoubleNear(12.0, 0.01));
  EXPECT_THAT(value->units().value_or('x'), ::testing::Eq('M'));
  EXPECT_THAT(value->converted().value(),
              ::testing::DoubleNear(12.0 * 3.6, 0.01));
}

TEST_F(Suite, returns_correct_value_for_kmh) {

  wellnmea::Sentence sentence;

  sentence.fields.push_back("12.0");
  sentence.fields.push_back("K");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr->extract(it, end)->as<wellnmea::instructions::SpeedValue>();

  ASSERT_TRUE(value->raw().has_value());
  EXPECT_THAT(value->raw().value(), ::testing::DoubleNear(12.0, 0.01));
  EXPECT_THAT(value->units().value_or('x'), ::testing::Eq('K'));
  EXPECT_THAT(value->converted().value(), ::testing::DoubleNear(12.0, 0.01));
}

