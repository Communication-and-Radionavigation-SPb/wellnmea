#pragma once

#include <optional>

#include <wellnmea/instructions/instruction.hpp>

#include <wellnmea/instructions/number_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

namespace wellnmea {
namespace instructions {
class SpeedValue : public NullValue {

  std::optional<char> units_;
  std::optional<double> speed_;

 public:
  SpeedValue(const std::string& name) : NullValue(name) {}

 public:

  void accept(visitor_base& v) const noexcept {
    using value_visitor = visitor<SpeedValue>;
    if (value_visitor* ev = dynamic_cast<value_visitor*>(&v)) {
      ev->visit(this);
    }
  }
  void set(std::optional<char> u, std::optional<double> s) noexcept {
    if (u.has_value() && s.has_value()) {
      units_ = u;
      speed_ = s;
      return;
    }
    units_ = std::nullopt;
    speed_ = std::nullopt;
  }
  std::optional<char> units() const noexcept { return units_; }

  std::optional<double> raw() const noexcept { return speed_; }

  std::optional<double> converted() const noexcept {
    if (!units_.has_value())
      return std::nullopt;

    if (units_.value() == 'N') {
      return speed_.value() * 1.852;
    }
    if (units_.value() == 'M') {
      return speed_.value() * 3.6;
    }
    return speed_;
  }
};

class SpeedInstruction : public Instruction {
 protected:
  NumberInstruction* num;
  SymbolInstruction* units;

  std::optional<char> unitOverride_;

 public:
  SpeedInstruction(const std::string& name,
                   std::optional<char> unitsOverride = std::nullopt)
      : Instruction(name),
        num(new NumberInstruction("value")),
        units(new SymbolInstruction("units", {'N', 'K', 'M'})),
        unitOverride_(unitsOverride) {}

 public:
  std::string which() const noexcept override { return "speed"; }

  Instruction* clone(const std::string& name) const override {
    return new SpeedInstruction(name, unitOverride_);
  }

  value* extract(position it, const_position end) override {

    auto result = new SpeedValue(name());

    auto number = num->extract(it, end)->as<NumberValue>();

    std::optional<char> unit;
    if (!unitOverride_.has_value()) {
      unit = units->extract(it, end)->as<CharacterValue>()->symbol();
    }
    else {
      unit = unitOverride_;
    }

    result->set(unit, number->getValue());

    return result;
  }
};
}  // namespace instructions

}  // namespace wellnmea
