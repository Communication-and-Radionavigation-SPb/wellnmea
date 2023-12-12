#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include <wellnmea/instructions/as_is_instruction.hpp>

#define Suite AsIsInstructionTests

namespace wai = wellnmea::instructions;

TEST(Suite, can_be_instantiated) {
  EXPECT_NO_THROW({ wai::AsIsInstruction instr("transduser"); });
}

TEST(Suite, stores_name_correctly) {
  wai::AsIsInstruction direction("direction");
  wai::AsIsInstruction movement("movement");

  EXPECT_EQ(direction.name(), "direction");
  EXPECT_EQ(movement.name(), "movement");
}

TEST(Suite, can_be_cloned) {
  wai::AsIsInstruction* instr = new wai::AsIsInstruction("direction");

  auto clone = instr->clone("movement");

  ASSERT_NE(clone, nullptr);
  EXPECT_NE(clone, instr);

  EXPECT_EQ(clone->name(), "movement");
}

TEST(Suite, moves_iterator_forward) {
  wai::AsIsInstruction instr("direction");

  wellnmea::Sentence sentence;

  sentence.fields.push_back("WTHI");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr.extract(it, end);

  EXPECT_EQ(it, end);
}

TEST(Suite, extract_returns_non_nullable_object) {
  wai::AsIsInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "WTHI";
  sentence.fields.push_back(
      std::string_view{field_value.c_str(), field_value.size()});

  auto it = sentence.fields.begin();

  auto value = instr.extract(it, sentence.fields.end());

  EXPECT_NE(value, nullptr);
}

TEST(Suite, assigns_instruction_name_to_value) {
  wai::AsIsInstruction direction("direction");
  wai::AsIsInstruction movement("movement");

  wellnmea::Sentence sentence;

  std::string field_value = "WTHI";
  sentence.fields.push_back(field_value);

  auto dit = sentence.fields.begin();
  auto mit = sentence.fields.begin();

  auto direction_value = direction.extract(dit, sentence.fields.end());
  auto movement_value = movement.extract(mit, sentence.fields.end());

  EXPECT_EQ(direction_value->name(), "direction");
  EXPECT_EQ(movement_value->name(), "movement");
}

TEST(Suite, extracts_field_value) {
  wai::AsIsInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "WTHI";
  sentence.fields.push_back(field_value);

  auto it = sentence.fields.begin();

  auto value = instr.extract(it, sentence.fields.end());

  ASSERT_NE(value->as<wai::AsIsValue>(), nullptr);
  ASSERT_TRUE(value->as<wai::AsIsValue>()->contents().has_value());
}

TEST(Suite, extracts_correct_field_value) {
  wai::AsIsInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "WTHI";
  std::string field_value_second = "PITCH";
  sentence.fields.push_back(field_value);
  sentence.fields.push_back(field_value_second);

  auto it = sentence.fields.begin();

  auto tvalue = instr.extract(it, sentence.fields.end())->as<wai::AsIsValue>();
  auto vvalue = instr.extract(it, sentence.fields.end())->as<wai::AsIsValue>();

  EXPECT_EQ(tvalue->contents().value(), field_value);
  EXPECT_EQ(vvalue->contents().value(), field_value_second);
}

TEST(Suite, assigns_nullopt_when_field_empty) {
  wai::AsIsInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "";
  sentence.fields.push_back(field_value);

  auto it = sentence.fields.begin();

  auto value = instr.extract(it, sentence.fields.end());

  ASSERT_NE(value->as<wai::AsIsValue>(), nullptr);
  ASSERT_FALSE(value->as<wai::AsIsValue>()->contents().has_value());
}