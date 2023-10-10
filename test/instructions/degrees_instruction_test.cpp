#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>
#include <wellnmea/instructions/degrees_instruction.hpp>

#define Suite DegreesInstructionTest

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wellnmea::instructions::DegreesInstruction instr("");
  });
}

TEST(Suite, can_be_cloned)
{
  wellnmea::instructions::DegreesInstruction *instr = new wellnmea::instructions::DegreesInstruction("base");

  auto clone = instr->clone("clone");

  EXPECT_EQ(clone->name(), "clone");
  EXPECT_NE(clone, instr);

  delete instr;
  delete clone;
}

TEST(Suite, which_expects_to_be_correct)
{
  wellnmea::instructions::DegreesInstruction instr("name");
  EXPECT_EQ(instr.which(), "degrees");
}

TEST(Suite, moves_iterator_forward)
{
  wellnmea::instructions::DegreesInstruction instr("name");

  wellnmea::Sentence sentence;
  wellnmea::Parser parser;

  parser.parseInto(sentence, "$GPHDT,127.0");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  instr.extract(it, end);

  EXPECT_NE(it, sentence.fields.begin()) << "Instruction do not moves iterator forward";
}

TEST(Suite, extract_returns_non_nullable_object)
{
  wellnmea::instructions::DegreesInstruction instr("name");

  wellnmea::Sentence sentence;
  wellnmea::Parser parser;

  parser.parseInto(sentence, "$GPHDT,127.0");

  auto it = sentence.fields.begin();
  auto end = sentence.fields.end();

  auto value = instr.extract(it, end);

  ASSERT_NE(value, nullptr);
  delete value;
}

TEST(Suite, extracted_value_has_correct_name)
{
  wellnmea::instructions::DegreesInstruction instr("name");

  wellnmea::Sentence sentence;
  wellnmea::Parser parser;

  parser.parseInto(sentence, "$GPHDT,127.02");

  auto it = sentence.fields.begin();

  auto value = instr.extract(it, sentence.fields.end());

  EXPECT_EQ(value->name(), "degrees");
}

TEST(Suite, extracts_correct_value_from_sentence)
{
  wellnmea::instructions::DegreesInstruction instr("name");

  wellnmea::Sentence sentence;
  wellnmea::Parser parser;

  parser.parseInto(sentence, "$GPHDT,127.0");

  auto it = sentence.fields.begin();

  auto value = instr.extract(it, sentence.fields.end());

  EXPECT_EQ(value->as<wellnmea::instructions::DegreesValue>()->value.has_value(), true) << "Instruction do not fills resulting value";
  EXPECT_EQ(value->as<wellnmea::instructions::DegreesValue>()->value.value(), 127.0) << "Instruction do not fills resulting value";
}

TEST(Suite, throws_on_invalid_field_content)
{
  wellnmea::instructions::DegreesInstruction instr("name");

  wellnmea::Sentence sentence;
  wellnmea::Parser parser;

  parser.parseInto(sentence, "$GPHDT,error_content");

  auto it = sentence.fields.begin();

  EXPECT_THROW({
    instr.extract(it, sentence.fields.end());
  },
               wellnmea::NumberDecodeError);
}