#pragma once
#include <gmock/gmock.h>
#include <wellnmea/formats/format.hpp>
#include <wellnmea/instructions/instruction.hpp>

class FakeValue : public wellnmea::instructions::NullValue
{
public:
  FakeValue() : wellnmea::instructions::NullValue("fake") {}
};

class MockedInstruction : public wellnmea::instructions::Instruction
{
public:
  MockedInstruction() : wellnmea::instructions::Instruction("") {}
  virtual ~MockedInstruction() = default;

public:
  std::string which() const noexcept override
  {
    return "mocked";
  }

public:
  // MOCK_METHOD(std::string, which, (), (const, noexcept, override));
  MOCK_METHOD(value *, extract, (position, const_position), (override));
  MOCK_METHOD(wellnmea::instructions::Instruction *, clone, (const std::string &), (const, override));
};

class MockedFormat : public wellnmea::formats::Format
{
public:
  MockedFormat() : wellnmea::formats::Format({new MockedInstruction()}) {}

public:
  MOCK_METHOD(std::list<ValuePtr>, parse, (wellnmea::Sentence &), (const, override));
};