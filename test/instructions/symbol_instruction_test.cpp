#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/sentence.hpp>
#include <wellnmea/parser.hpp>

#include <wellnmea/instructions/symbol_instruction.hpp>

#define Suite SymbolInstructionTest

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wellnmea::instructions::SymbolInstruction instr("direction");
  });
}

TEST(Suite, which_returns_correct_value)
{
  wellnmea::instructions::SymbolInstruction instr("direction");

  EXPECT_EQ(instr.which(), "char");
}

TEST(Suite, stores_name_correctly)
{
  wellnmea::instructions::SymbolInstruction direction("direction");
  wellnmea::instructions::SymbolInstruction movement("movement");

  EXPECT_EQ(direction.name(), "direction");
  EXPECT_EQ(movement.name(), "movement");
}

TEST(Suite, can_be_cloned)
{
  wellnmea::instructions::SymbolInstruction *instr = new wellnmea::instructions::SymbolInstruction("direction");

  auto clone = instr->clone("movement");

  ASSERT_NE(clone, nullptr);
  EXPECT_NE(clone, instr);

  EXPECT_EQ(clone->name(), "movement");
}

TEST(Suite, moves_iterator_forward)
{
  wellnmea::instructions::SymbolInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "T";
  sentence.fields.push_back(std::string_view{field_value.c_str(), field_value.size()});

  auto it = sentence.fields.begin();

  instr.extract(it, sentence.fields.end());

  EXPECT_NE(it, sentence.fields.begin());
}

TEST(Suite, extract_returns_non_nullable_object)
{
  wellnmea::instructions::SymbolInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "T";
  sentence.fields.push_back(std::string_view{field_value.c_str(), field_value.size()});

  auto it = sentence.fields.begin();

  auto value = instr.extract(it, sentence.fields.end());

  EXPECT_NE(value, nullptr);
}

TEST(Suite, assigns_instruction_name_to_value)
{
  wellnmea::instructions::SymbolInstruction direction("direction");
  wellnmea::instructions::SymbolInstruction movement("movement");

  wellnmea::Sentence sentence;

  std::string field_value = "T";
  sentence.fields.push_back(field_value);

  auto dit = sentence.fields.begin();
  auto mit = sentence.fields.begin();

  auto direction_value = direction.extract(dit, sentence.fields.end());
  auto movement_value = movement.extract(mit, sentence.fields.end());

  EXPECT_EQ(direction_value->name(), "direction");
  EXPECT_EQ(movement_value->name(), "movement");
}

TEST(Suite, extracts_field_value)
{
  wellnmea::instructions::SymbolInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "T";
  sentence.fields.push_back(field_value);

  auto it = sentence.fields.begin();

  auto value = instr.extract(it, sentence.fields.end());

  ASSERT_NE(value->as<wellnmea::instructions::CharacterValue>(), nullptr);
  ASSERT_TRUE(value->as<wellnmea::instructions::CharacterValue>()->value.has_value());
}

TEST(Suite, extracts_correct_field_value)
{
  wellnmea::instructions::SymbolInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "T";
  std::string field_value_second = "V";
  sentence.fields.push_back(field_value);
  sentence.fields.push_back(field_value_second);

  auto it = sentence.fields.begin();

  auto tvalue = instr.extract(it, sentence.fields.end());
  auto vvalue = instr.extract(it, sentence.fields.end());

  EXPECT_EQ(tvalue->as<wellnmea::instructions::CharacterValue>()->value.value(), 'T');
  EXPECT_EQ(vvalue->as<wellnmea::instructions::CharacterValue>()->value.value(), 'V');
}

TEST(Suite, throws_when_field_content_is_not_valid_for_character)
{
  wellnmea::instructions::SymbolInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "3";

  sentence.fields.push_back(field_value);

  auto it = sentence.fields.begin();
  EXPECT_THROW({
    instr.extract(it, sentence.fields.end());
  },
               wellnmea::extraction_error);
}

TEST(Suite, throws_when_field_content_is_not_single_character)
{
  wellnmea::instructions::SymbolInstruction instr("direction");

  wellnmea::Sentence sentence;

  std::string field_value = "TT";

  sentence.fields.push_back(field_value);

  auto it = sentence.fields.begin();
  EXPECT_THROW({
    instr.extract(it, sentence.fields.end());
  },
               wellnmea::extraction_error);
}

TEST(Suite, throws_when_not_in_allowed_chars)
{
  wellnmea::instructions::SymbolInstruction instr("direction", {'T', 'M'});

  wellnmea::Sentence sentence;

  std::string field_value = "E";

  sentence.fields.push_back(field_value);

  auto it = sentence.fields.begin();
  EXPECT_THROW({
    instr.extract(it, sentence.fields.end());
  },
               wellnmea::extraction_error);
}