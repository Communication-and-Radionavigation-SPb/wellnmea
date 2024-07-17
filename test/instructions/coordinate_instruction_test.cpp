#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "helpers.hpp"

#include <wellnmea/sentence.hpp>
#include <wellnmea/instructions/coordinate_instruction.hpp>

#define Suite CoordinateInstructionTests

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wellnmea::instructions::CoordinateInstruction instr("");
  });
}

TEST(Suite, can_be_cloned)
{
  test_clonable<wellnmea::instructions::CoordinateInstruction>();
}

TEST(Suite, which_expected_to_be_correct)
{
  wellnmea::instructions::CoordinateInstruction instr("");

  EXPECT_EQ(instr.which(), "coordinate");
}

TEST(Suite, moves_iterator_forward)
{
  wellnmea::instructions::CoordinateInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("12");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr.extract(it, end);

  EXPECT_EQ(it, ++sentence.fields.begin());
}

TEST(Suite, returns_non_null_value)
{
  wellnmea::instructions::CoordinateInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);

  EXPECT_NE(value, nullptr);
}

TEST(Suite, returned_value_has_correct_name)
{
  wellnmea::instructions::CoordinateInstruction instr("latitude");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);
  EXPECT_EQ(value->name(), "latitude");
}

TEST(Suite, returns_empty_value_for_empty_string)
{
  wellnmea::instructions::CoordinateInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end)->as<wellnmea::instructions::CoordinateValue>();

  EXPECT_EQ(value->degrees(), std::nullopt);
  EXPECT_EQ(value->minutes(), std::nullopt);
}

TEST(Suite, correctly_parses_value)
{
  wellnmea::instructions::CoordinateInstruction instr("latitude");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("4807.038");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end)->as<wellnmea::instructions::CoordinateValue>();

  EXPECT_THAT(value->degrees(), ::testing::Eq(48));
  EXPECT_THAT(value->minutes().value(), ::testing::DoubleNear(7.038, 0.001));
}

TEST(Suite, coordinate_value_returns_correct_degrees) {
  wellnmea::instructions::CoordinateValue value("");

  value.setBase(4807.038);
  EXPECT_THAT(value.degrees().value(), ::testing::Eq(48));

  value.setBase(7.038);
  EXPECT_THAT(value.degrees().value(), ::testing::Eq(0));
}

TEST(Suite, coordinate_value_returns_correct_minutes) {
  wellnmea::instructions::CoordinateValue value("");

  value.setBase(8.0);
  EXPECT_THAT(value.minutes().value(), ::testing::DoubleNear(8.0, 0));
  
  value.setBase(4800.0);
  EXPECT_THAT(value.minutes().value(), ::testing::DoubleNear(0.0, 0));
}

TEST(Suite, coordinate_value_understands_negative_base) {
  wellnmea::instructions::CoordinateValue value("");

  value.setBase(-4807.035);

  EXPECT_THAT(value.degrees().value(), ::testing::Eq(-48));
  EXPECT_THAT(value.minutes().value(), ::testing::DoubleNear(-7.035, 0.001));
}