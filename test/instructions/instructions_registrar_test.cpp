#include <gtest/gtest.h>

#include <wellnmea/formats/instructions_registry.hpp>
#include <wellnmea/formats/degreese_instruction.hpp>
#include <wellnmea/formats/longitude_instruction.hpp>
#include <wellnmea/instructions_registrar.h>

#define Suite InstructionsRegistrarTest

using namespace wellnmea::formats;
using namespace wellnmea;

TEST(Suite, CorrectlyCreatesInstructions)
{
  InstructionsRegistry::clear();

  InstructionsRegistrar<
      DegreesInstruction,
      LongitudeInstruction>::use();

  EXPECT_TRUE(InstructionsRegistry::contains("degrees"));
  EXPECT_TRUE(InstructionsRegistry::contains("longitude"));
}

TEST(Suite, CorrectlyWorksWithNoTemplateArguments)
{
  InstructionsRegistry::clear();

  EXPECT_NO_THROW({
    InstructionsRegistrar<>::use();
  });
}

TEST(Suite, CorrectlyWorksWithOneArgument)
{
  InstructionsRegistry::clear();

  InstructionsRegistrar<
      DegreesInstruction>::use();

  EXPECT_TRUE(InstructionsRegistry::contains("degrees"));
}