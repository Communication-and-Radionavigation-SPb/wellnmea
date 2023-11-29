#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.hpp"

#include <wellnmea/instructions/distance_instruction.hpp>
#include <wellnmea/sentence.hpp>

#define Suite DistanceInstructionTests

class DistanceInstructionTests : public ::testing::Test {
 public:
  wellnmea::instructions::DistanceInstruction* instr;

 protected:
  void SetUp() override {
    instr = new wellnmea::instructions::DistanceInstruction("distance");
  }

  void TearDown() override { delete instr; }
};

TEST_F(Suite, has_correct_name) {
  EXPECT_EQ(instr->name(), "distance");
}

TEST_F(Suite, can_be_cloned) {
  test_clonable<wellnmea::instructions::DistanceInstruction>();
}

TEST_F(Suite, which_expected_to_be_correct) {
  EXPECT_EQ(instr->which(), "distance");
}

TEST_F(Suite, moves_iterator_forward) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("3.9");
  sentence.fields.push_back("f");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr->extract(it, end);

  EXPECT_NE(it, sentence.fields.begin());
}

TEST_F(Suite, moves_iterator_two_steps_forward) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("001.2");
  sentence.fields.push_back("F");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr->extract(it, end);

  EXPECT_EQ(it, sentence.fields.end());
}

TEST_F(Suite, returns_nonnull_value) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("M");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr->extract(it, end);

  EXPECT_NE(value, nullptr);
}

TEST_F(Suite, returns_correct_value_for_feets) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("f");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr->extract(it, end)->as<wellnmea::instructions::DistanceValue>();

  ASSERT_TRUE(*value);
  ASSERT_NE(value, nullptr);
  EXPECT_THAT(value->raw().value(), ::testing::DoubleNear(56.0, 0.01));
  EXPECT_THAT(value->units().value(), ::testing::Eq('f'));
  EXPECT_THAT(value->converted().value(),
              ::testing::DoubleNear(56.0 * 0.3048, 0.0001));
}

TEST_F(Suite, returns_correct_value_for_meters) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("M");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr->extract(it, end)->as<wellnmea::instructions::DistanceValue>();

  ASSERT_TRUE(*value);
  ASSERT_NE(value, nullptr);
  EXPECT_THAT(value->raw().value(), ::testing::DoubleNear(56.0, 0.01));
  EXPECT_THAT(value->units().value(), ::testing::Eq('M'));
  EXPECT_THAT(value->converted().value(), ::testing::DoubleNear(56.0, 0.01));
}

TEST_F(Suite, returns_correct_value_for_fathoms) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("56.0");
  sentence.fields.push_back("F");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value =
      instr->extract(it, end)->as<wellnmea::instructions::DistanceValue>();

  ASSERT_TRUE(*value);
  ASSERT_NE(value, nullptr);
  EXPECT_THAT(value->raw().value(), ::testing::DoubleNear(56.0, 0.01));
  EXPECT_THAT(value->units().value(), ::testing::Eq('F'));
  EXPECT_THAT(value->converted().value(),
              ::testing::DoubleNear(56.0 * 1.8288, 0.00001));
}

TEST_F(Suite, works_with_predefined_units) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("3.6");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  delete instr;
  instr = new wellnmea::instructions::DistanceInstruction("predef", 'M');

  auto value =
      instr->extract(it, end)->as<wellnmea::instructions::DistanceValue>();

  ASSERT_NE(value, nullptr);
  ASSERT_TRUE(*value);
  EXPECT_THAT(value->raw().value(), ::testing::DoubleNear(3.6, 0.01));
  EXPECT_THAT(value->units().value(), ::testing::Eq('M'));

  EXPECT_THAT(value->converted().value(), ::testing::DoubleNear(3.6, 0.01));
}