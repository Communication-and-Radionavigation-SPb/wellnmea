#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>
#include <wellnmea/instructions/utc_instruction.hpp>

#include "helpers.hpp"

#define Suite UtcInstructionTest

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wellnmea::instructions::UtcInstruction instr("");
  });
}

TEST(Suite, can_be_cloned)
{
  test_clonable<wellnmea::instructions::UtcInstruction>();
}

TEST(Suite, which_expects_to_be_correct)
{
  wellnmea::instructions::UtcInstruction instr("");
  EXPECT_EQ(instr.which(), "utc");
}

TEST(Suite, moves_iterator_forward)
{
  wellnmea::instructions::UtcInstruction instr("");

  wellnmea::Sentence sentence;
  sentence.fields.push_back("123200.00");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr.extract(it, end);

  EXPECT_EQ(it, end);
}

TEST(Suite, extract_returns_non_nullable_address)
{
  wellnmea::instructions::UtcInstruction instr("");

  wellnmea::Sentence sentence;
  sentence.fields.push_back("123200.00");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);
  EXPECT_NE(value, nullptr);
  delete value;
}

TEST(Suite, extracted_value_has_correct_name)
{
  wellnmea::instructions::UtcInstruction instr("");

  wellnmea::Sentence sentence;
  sentence.fields.push_back("123200.00");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);

  EXPECT_EQ(value->name(), "utc");
}

TEST(Suite, extracts_correct_value_from_sentence)
{
  wellnmea::instructions::UtcInstruction instr("name");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("132311.623");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);

  EXPECT_EQ(value->as<wellnmea::instructions::UtcValue>()->getTimestamp().value(), 132311.623);
}

TEST(Suite, utc_value_returns_nullopt_when_do_not_have_timestamp)
{
  wellnmea::instructions::UtcValue value("");

  EXPECT_EQ(value.getHours(), std::nullopt);
  EXPECT_EQ(value.getSeconds(), std::nullopt);
  EXPECT_EQ(value.getMinutes(), std::nullopt);
  EXPECT_EQ(value.getMilliseconds(), std::nullopt);
}

TEST(Suite, utc_value_returns_hours)
{
  wellnmea::instructions::UtcValue value("");

  value.setTimestamp(123211.623);
  EXPECT_EQ(value.getHours(), 12);


  value.setTimestamp(3020.623);
  EXPECT_EQ(value.getHours(), 0);
}

TEST(Suite, utc_value_returns_minutes)
{
  wellnmea::instructions::UtcValue value("");

  value.setTimestamp(3211.623);
  EXPECT_EQ(value.getMinutes(), 32);

  value.setTimestamp(2000.0);
  EXPECT_EQ(value.getMinutes(), 20);

  value.setTimestamp(10.0);
  EXPECT_EQ(value.getMinutes(), 0);

  value.setTimestamp(120010.0);
  EXPECT_EQ(value.getMinutes(), 0);
}

TEST(Suite, utc_value_returns_seconds) {
  wellnmea::instructions::UtcValue value("");

  value.setTimestamp(11.623);
  EXPECT_EQ(value.getSeconds(), 11);

  value.setTimestamp(30.623);
  EXPECT_EQ(value.getSeconds(), 30);

  value.setTimestamp(0.625);
  EXPECT_EQ(value.getSeconds(), 0);
}

TEST(Suite, utc_value_returns_milliseconds) {
  wellnmea::instructions::UtcValue value("");

  value.setTimestamp(0.625);

  EXPECT_EQ(value.getMilliseconds(), 625);

}
