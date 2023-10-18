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

 public:
  SpeedInstruction(const std::string& name)
      : Instruction(name),
        num(new NumberInstruction("value")),
        units(new SymbolInstruction("units", {'N', 'K', 'M'})) {}

 public:
  std::string which() const noexcept override { return "speed"; }

  Instruction* clone(const std::string& name) const override {
    return new SpeedInstruction(name);
  }

  value* extract(position it, const_position end) override {

    auto value = new SpeedValue(name());

    auto number = num->extract(it, end)->as<NumberValue>();
    auto unit = units->extract(it, end)->as<CharacterValue>();

    value->set(unit->symbol(), number->getValue());

    return value;
  }
};
}  // namespace instructions

}  // namespace wellnmea
