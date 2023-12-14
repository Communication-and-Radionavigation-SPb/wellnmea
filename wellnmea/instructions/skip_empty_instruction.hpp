#pragma once

#include <sstream>
#include <wellnmea/instructions/instruction.hpp>

namespace wellnmea {
namespace instructions {

class SkipInstruction : public Instruction {
 public:
  SkipInstruction(size_t fields) : Instruction(""), size(fields) {}

 public:
  std::string which() const noexcept override { return "skip"; }

  Instruction* clone(const std::string& name) const override {
    return new SkipInstruction(size);
  }

  value* extract(position it, const_position end) override {
    for (size_t i = 0; i < size; i++) {
      if (it == end)
        throw size_error("not anouth sentence fields");
      if (!it->empty()) {
        std::stringstream ss;
        ss << "can not skip non-empty fields but `" << *it << "` passed";
        throw extraction_error(ss.str());
      }
      ++it;
    }
    return nullptr;
  }

 private:
  size_t size;
};

}  // namespace instructions

}  // namespace wellnmea
