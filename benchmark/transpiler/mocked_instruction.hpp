#pragma once
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
  MockedInstruction() : wellnmea::instructions::Instruction("mock") {}
  virtual ~MockedInstruction() = default;

public:
  std::string which() const noexcept override
  {
    return "mocked";
  }

  value *extract(position it, const_position end) override
  {
    it++;
    return new FakeValue();
  }

  Instruction *clone(const std::string &name) const override
  {
    return new MockedInstruction();
  }
};
