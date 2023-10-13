#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/sentence.hpp>

void move_iterator(wellnmea::instructions::Instruction::position it) {
  ++it;
}

void double_move(wellnmea::instructions::Instruction::position it)
{
  move_iterator(it);
  move_iterator(it);
}

#define Suite IterationTest

TEST(Suite, double_move_test) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  double_move(it);

  EXPECT_EQ(it, end);
}
