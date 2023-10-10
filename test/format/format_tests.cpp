#include <gtest/gtest.h>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/instructions/degrees_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>
#include <wellnmea/formats/format.hpp>

#include <wellnmea/sentence.hpp>

#define Suite FormatTest

namespace wn
{
  using namespace wellnmea;
  using namespace wellnmea::instructions;
}

TEST(Suite, throws_when_empty_instructions_list_passed)
{
  EXPECT_DEATH({
    wn::formats::Format({});
  },
               "");
}

TEST(Suite, uses_passed_instructions)
{

  wn::Sentence sentence;
  sentence.fields.push_back("49.5");
  sentence.fields.push_back("M");

  auto fmt = wn::formats::Format({new wn::DegreesInstruction("direction"),
                                  new wn::SymbolInstruction("heading", {'M', 'T'})});

  auto result = fmt.parse(sentence);

  ASSERT_EQ(result.size(), 2);

  auto it = result.begin();
  EXPECT_NE((*it)->as<wn::DegreesValue>(), nullptr);
  EXPECT_EQ((*it)->as<wn::DegreesValue>()->value.value(), 49.5);

  it++;

  EXPECT_NE((*it)->as<wn::CharacterValue>(), nullptr);
  EXPECT_EQ((*it)->as<wn::CharacterValue>()->value.value(), 'M');
}