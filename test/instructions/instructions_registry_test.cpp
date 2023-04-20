#include <gtest/gtest.h>

#include <wellnmea/formats/instructions_registry.hpp>

#define Suite InstructionsRegistry

using namespace wellnmea;
using namespace wellnmea::formats;

TEST(Suite, CanCreateInstructionsByTemplateArguments) {
  InstructionsRegistry::clear();
}