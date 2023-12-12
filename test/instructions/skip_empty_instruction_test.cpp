#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "helpers.hpp"

#include <wellnmea/instructions/skip_empty_instruction.hpp>
#include <wellnmea/sentence.hpp>

#define Suite SkipInstructionTests

class SkipInstructionTests : public testing::Test {
 public:
  wellnmea::instructions::SkipInstruction* instr;

 protected:
  void SetUp() override {
    instr = new wellnmea::instructions::SkipInstruction(3);
  }

  void TearDown() override { delete instr; }
};

TEST_F(Suite, has_correct_name) {
  EXPECT_EQ(instr->name(), "");
}

TEST_F(Suite, can_be_cloned) {
  auto newinst = instr->clone("other");
}

TEST_F(Suite, which_expected_to_be_correct) {
  EXPECT_EQ(instr->which(), "skip");
}

TEST_F(Suite, moves_iterator_forward) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr->extract(it, end);
  EXPECT_NE(it, sentence.fields.begin());
}

TEST_F(Suite, moves_iterator_three_steps_forward) {

  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr->extract(it, end);
  EXPECT_EQ(it, sentence.fields.end());
}

TEST_F(Suite, returns_null_value) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr->extract(it, end);

  EXPECT_EQ(value, nullptr);
}

TEST_F(Suite, throws_when_first_not_empty) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("a");
  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  EXPECT_ANY_THROW({ instr->extract(it, end); });
}

TEST_F(Suite, throws_when_middle_not_empty) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("a");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  EXPECT_ANY_THROW({ instr->extract(it, end); });
}

TEST_F(Suite, throws_when_last_not_empty) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("a");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  EXPECT_ANY_THROW({ instr->extract(it, end); });
}

TEST_F(Suite, throws_when_not_anouth_to_skip) {
  wellnmea::Sentence sentence;

  sentence.fields.push_back("");
  sentence.fields.push_back("");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  EXPECT_ANY_THROW({ instr->extract(it, end); });
}
