#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "helpers.hpp"

#include <wellnmea/sentence.hpp>
#include <wellnmea/instructions/longitude_instruction.hpp>

#define LongitudeInstructionTests

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wellnmea::instructions::LongitudeInstruction instr("");
  });
}

TEST(Suite, can_be_cloned)
{
  test_clonable<wellnmea::instructions::LongitudeInstruction>();
}

TEST(Suite, which_expected_to_be_correct)
{
  wellnmea::instructions::LongitudeInstruction instr("");

  EXPECT_EQ(instr.which(), "longitude");
}

TEST(Suite, moves_iterator_two_positions_forward)
{
  wellnmea::instructions::LongitudeInstruction instr("");

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
  wellnmea::instructions::LongitudeInstruction instr("");

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
  wellnmea::instructions::LongitudeInstruction instr("");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("4807.038");
  sentence.fields.push_back("E");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end)->as<wellnmea::instructions::LongitudeValue>();

  EXPECT_EQ(value->direction(), 'E');
  EXPECT_EQ(value->degrees(), 48.0);
  EXPECT_THAT(value->minutes().value(), ::testing::DoubleNear(7.038, 0.001));
}

TEST(Suite, latitude_value_correctly_accepts_south_direction)
{

  wellnmea::instructions::LongitudeValue value("");

  value.setDirection('W');
  value.setBase(4807.035);

  EXPECT_EQ(value.direction(), 'W');
  EXPECT_THAT(value.degrees(), ::testing::Lt(0));
  EXPECT_THAT(value.minutes(), ::testing::Lt(0));

  value.reset();

  value.setBase(4807.035);
  value.setDirection('W');

  EXPECT_EQ(value.direction(), 'W');
  EXPECT_THAT(value.degrees(), ::testing::Lt(0));
  EXPECT_THAT(value.minutes(), ::testing::Lt(0));
}