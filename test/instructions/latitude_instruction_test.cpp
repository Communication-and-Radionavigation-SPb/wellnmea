#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "helpers.hpp"

#include <wellnmea/sentence.hpp>
#include <wellnmea/instructions/latitude_instruction.hpp>

#define Suite LatitudeInstructionTests

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wellnmea::instructions::LatitudeInstruction instr("");
  });
}

TEST(Suite, can_be_cloned)
{
  test_clonable<wellnmea::instructions::LatitudeInstruction>();
}

TEST(Suite, which_expected_to_be_correct)
{
  wellnmea::instructions::LatitudeInstruction instr("");

  EXPECT_EQ(instr.which(), "latitude");
}

TEST(Suite, moves_iterator_two_positions_forward)
{
  wellnmea::instructions::LatitudeInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr.extract(it, end);
  EXPECT_EQ(it, end);
}

TEST(Suite, returns_non_null_value)
{
  wellnmea::instructions::LatitudeInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);

  EXPECT_NE(value, nullptr);
}

TEST(Suite, returns_correct_value)
{
  wellnmea::instructions::LatitudeInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("4807.038");
  sentence.fields.push_back("N");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end)->as<wellnmea::instructions::LatitudeValue>();

  EXPECT_EQ(value->degrees(), 48.0);
  EXPECT_THAT(value->minutes().value(), ::testing::DoubleNear(7.038, 0.001));
  EXPECT_EQ(value->direction(), 'N');
}

TEST(Suite, latitude_value_correctly_accepts_noth_direction)
{
  wellnmea::instructions::LatitudeValue value("");

  value.setDirection('N');
  value.setBase(4807.035);

  EXPECT_EQ(value.direction(), 'N');
  EXPECT_THAT(value.degrees(), ::testing::Gt(0));
  EXPECT_THAT(value.minutes(), ::testing::Gt(0));

  value.reset();

  value.setBase(4807.035);
  value.setDirection('N');

  EXPECT_EQ(value.direction(), 'N');
  EXPECT_THAT(value.degrees(), ::testing::Gt(0));
  EXPECT_THAT(value.minutes(), ::testing::Gt(0));
}

TEST(Suite, latitude_value_correctly_accepts_south_direction)
{

  wellnmea::instructions::LatitudeValue value("");

  value.setDirection('S');
  value.setBase(4807.035);

  EXPECT_EQ(value.direction(), 'S');
  EXPECT_THAT(value.degrees(), ::testing::Lt(0));
  EXPECT_THAT(value.minutes(), ::testing::Lt(0));

  value.reset();

  value.setBase(4807.035);
  value.setDirection('S');

  EXPECT_EQ(value.direction(), 'S');
  EXPECT_THAT(value.degrees(), ::testing::Lt(0));
  EXPECT_THAT(value.minutes(), ::testing::Lt(0));
}
