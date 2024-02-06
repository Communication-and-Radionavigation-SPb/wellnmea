#pragma once

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>
#include <wellnmea/util/string_utils.hpp>

namespace wellnmea {

namespace instructions {

/* -------------------------------------------------------------------------- */
/*                                Degrees Value                               */
/* -------------------------------------------------------------------------- */
class DegreesValue : public NullValue {
 public:
  using visitor_t = visitor<DegreesValue>;
  /* ------------------------------- Attributes ------------------------------- */
 public:
  std::optional<double> value;

 public:
  DegreesValue(std::string const& name = "degrees") : NullValue(name) {}

 public:
  virtual void accept(visitor_base& v) const noexcept override {
    visitor_t* ev = dynamic_cast<visitor_t*>(&v);
    if (ev != nullptr)
      ev->visit(this);
  }
};

/* -------------------------------------------------------------------------- */
/*                                 Instruction                                */
/* -------------------------------------------------------------------------- */
class DegreesInstruction : public Instruction {
 public:
  DegreesInstruction(const std::string& name) : Instruction(name) {}

 public:
  std::string which() const noexcept override { return "degrees"; }

  value* extract(position it, const_position end) override {
    auto value = new DegreesValue(name());

    // Assign degrees value
    value->value = util::toDouble(std::string{it->begin(), it->end()});

    // Move iterator forward
    it++;
    return value;
  }

  Instruction* clone(const std::string& name) const override {
    return new DegreesInstruction(name);
  }
};
}  // namespace instructions

}  // namespace wellnmea
