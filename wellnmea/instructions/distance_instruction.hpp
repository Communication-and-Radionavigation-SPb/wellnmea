#pragma once

#include <optional>

#include <wellnmea/instructions/instruction.hpp>

#include <wellnmea/instructions/number_instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

namespace wellnmea {
namespace instructions {
class DistanceValue : public NullValue {
 private:
  std::optional<char> units_;
  std::optional<double> distance_;

 public:
  DistanceValue(const std::string& name) : NullValue(name) {}

 public:
  void accept(visitor_base& v) const noexcept {
    using value_visitor = visitor<DistanceValue>;
    if (value_visitor* ev = dynamic_cast<value_visitor*>(&v)) {
      ev->visit(this);
    }
  }

  void set(std::optional<char> u, std::optional<double> v) noexcept {
    if (u.has_value() && v.has_value()) {
      units_ = u;
      distance_ = v;
      return;
    }
    units_ = std::nullopt;
    distance_ = std::nullopt;
  }

  std::optional<char> units() const noexcept { return units_; }

  std::optional<double> raw() const noexcept { return distance_; };

  std::optional<double> converted() const noexcept {
    if (!units_.has_value())
      return std::nullopt;

    if (units_.value() == 'f') { // fathoms to meters
      return distance_.value() * 0.3048;
    }

    if (units_.value() == 'F') { // feet to meters
      return distance_.value() * 1.8288;
    }

    return distance_;
  }

  operator bool() const { return units_.has_value() && distance_.has_value(); }
};

class DistanceInstruction : public Instruction {
 protected:
  NumberInstruction* num;
  SymbolInstruction* units;

  std::optional<char> unitOverride_;

 public:
 public:
  DistanceInstruction(const std::string& name,
                      std::optional<char> unitsOverride = std::nullopt)
      : Instruction(name),
        num(new NumberInstruction("value")),
        units(new SymbolInstruction("units", {'f', 'M', 'F'})),
        unitOverride_(unitsOverride) {
    if (unitOverride_.has_value()) {
      const std::list<char> units = {'f', 'M', 'F'};
      auto pos = std::find(units.begin(), units.end(), unitOverride_.value());
      assert(pos != units.end());
    }
  }

 public:
  std::string which() const noexcept override { return "distance"; }

  Instruction* clone(const std::string& name) const override {
    return new DistanceInstruction(name, unitOverride_);
  }

  value* extract(position it, const_position end) override {

    auto value = new DistanceValue(name());
    auto number = num->extract(it, end)->as<NumberValue>();

    std::optional<char> unit;
    if (unitOverride_.has_value()) {
      unit = unitOverride_;
    }
    else {
      unit = units->extract(it, end)->as<CharacterValue>()->symbol();
    }

    value->set(unit, number->getValue());

    return value;
  }
};
}  // namespace instructions
}  // namespace wellnmea
