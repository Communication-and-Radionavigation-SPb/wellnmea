#pragma once

#include <string>
#include <wellnmea/instructions/instruction.hpp>

class ShotValue : public wellnmea::instructions::NullValue
{
public:
  ShotValue(const std::string &name) : NullValue(name) {}
};

class ShotInstruction : public wellnmea::instructions::Instruction
{

public:
  ShotInstruction() : Instruction("shot") {}

public:
  std::string which() const noexcept override
  {
    return "shot";
  }

  value *extract(position it, const_position end) override
  {
    return new ShotValue(name());
  }

  /// Mock
  Instruction *clone(const std::string &other) const override
  {
    return nullptr;
  }
};