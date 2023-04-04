#pragma once

#include <wellnmea/formats/format.hpp>

class fake_param: public wellnmea::BaseValue {

};

class fake_format: public wellnmea::formats::Instruction {
  public:
  fake_format() = default;
  ~fake_format()  = default;

  Instruction::value extract(const std::list<Token>::iterator it) override {
    return std::make_shared<wellnmea::BaseValue>(new fake_param());
  }
};