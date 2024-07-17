#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/instructions/number_instruction.hpp>
#include <wellnmea/instructions/repeated_instruction.hpp>
#include <wellnmea/sentence.hpp>

#define Suite RepeatedInstructionTest

namespace wa = wellnmea;
namespace wai = wellnmea::instructions;

class RepeatedInstructionTest : public ::testing::Test {
 public:
  wai::Instruction* create(
      const wai::RepeatedInstruction::collection_t& collection) {
    return new wai::RepeatedInstruction("test", collection);
  }
};

TEST_F(Suite, can_be_instantiated) {
  EXPECT_NO_THROW({ wai::RepeatedInstruction("name", {}); });
}

TEST_F(Suite, has_correct_name) {
  auto instr = wai::RepeatedInstruction("name", {});

  EXPECT_EQ(instr.name(), "name");
}

TEST_F(Suite, moves_iterator_forward) {
  auto i = create({new wai::NumberInstruction("value")});

  wa::Sentence sentence;

  std::vector<double> values{12.0, 13.0};
  for (auto&& v : values) {
    sentence.fields.push_back(std::to_string(v));
  }

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  i->extract(it, end);

  EXPECT_EQ(it, end);
}

TEST_F(Suite, extracts_correct_amout_of_values) {

  auto i = create({new wai::NumberInstruction("value")});

  wa::Sentence sentence;

  std::vector<double> values{12.0, 13.0};
  for (auto&& v : values) {
    sentence.fields.push_back(std::to_string(v));
  }

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = i->extract(it, end)->as<wai::RepeatedValue>();

  ASSERT_EQ(value->size(), 2);
}

TEST_F(Suite, works_properly_for_multiple_repeated_fields) {

  auto i = create({new wai::NumberInstruction("first"),
                   new wai::NumberInstruction("second")});

  wa::Sentence sentence;

  std::vector<double> values{12.0, 13.0, 6.0, 7.0};
  for (auto&& v : values) {
    sentence.fields.push_back(std::to_string(v));
  }

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = i->extract(it, end)->as<wai::RepeatedValue>();

  ASSERT_EQ(value->size(), 2);
}

TEST_F(Suite, each_repetition_has_correct_size) {
  auto i = create({new wai::NumberInstruction("first"),
                   new wai::NumberInstruction("second")});

  wa::Sentence sentence;

  std::vector<double> values{12.0, 13.0, 6.0, 7.0};
  for (auto&& v : values) {
    sentence.fields.push_back(std::to_string(v));
  }

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = i->extract(it, end)->as<wai::RepeatedValue>();

  auto pos = value->begin();
  while (pos != value->end()) {
    EXPECT_EQ(pos->size(), 2);
    pos++;
  }
}
