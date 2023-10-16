#pragma once

#include <wellnmea/instructions/coordinate_instruction.hpp>
#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/instructions/symbol_instruction.hpp>

#include <iostream>

namespace wellnmea {
namespace instructions {

/* -------------------------------------------------------------------------- */
/*                               Longitude value                              */
/* -------------------------------------------------------------------------- */

/**
     * @brief Value returned from <LongitudeInstruction>
     * 
     * Extends <CoordinateValue> class by providing 
     * possibility to set direction East or West.
     * 
     * If West direction provided then <CoordinateValue> part
     * will return negative numbers
     * 
     */
class LongitudeValue : public CoordinateValue {
 protected:
  std::optional<char> direction_;

 public:
  LongitudeValue(const std::string& name) : CoordinateValue(name) {}

 public:
  void accept(visitor_base& v) const noexcept override {
    using value_visitor = visitor<LongitudeValue>;
    if (value_visitor* ev = dynamic_cast<value_visitor*>(&v))
      ev->visit(this);
  }

  std::string_view name() const noexcept override {
    return CoordinateValue::name();
  }

  void setBase(std::optional<double> value) noexcept override {
    if (value.has_value() && direction_.value_or('x') == 'W') {
      value = value.value() * -1;
    }
    CoordinateValue::setBase(value);
  }

  void setDirection(std::optional<char> dir) noexcept {
    direction_ = dir;
    setBase(base);
  }

  std::optional<char> direction() const { return direction_; }

  void reset() noexcept override {
    CoordinateValue::reset();
    direction_ = std::nullopt;
  }

 public:
  LongitudeValue& operator=(const CoordinateValue& other) {
    static_cast<CoordinateValue&>(*this) = other;
    return *this;
  }
};

/* -------------------------------------------------------------------------- */
/*                            Longitude instruction                           */
/* -------------------------------------------------------------------------- */

/**
     * @brief Longitude parsing instruction
     * 
     * Would return <LongitudeValue> when extracted
     * 
     */
class LongitudeInstruction : public Instruction {
  CoordinateInstruction* coord;
  SymbolInstruction* direction;

 public:
  LongitudeInstruction(const std::string& name)
      : Instruction(name),
        coord(new CoordinateInstruction("longitude")),
        direction(new SymbolInstruction("direction", {'E', 'W'})) {}

  ~LongitudeInstruction() {
    delete coord;
    delete direction;
  }

 public:
  std::string which() const noexcept override { return "longitude"; }

  Instruction* clone(const std::string& name) const override {
    return new LongitudeInstruction(name);
  }

  value* extract(position it, const_position end) override {
    auto value = new LongitudeValue(name());

    auto coordinate = coord->extract(it, end);
    auto dir = direction->extract(it, end);

    *value = *coordinate->as<CoordinateValue>();

    value->setDirection(dir->as<CharacterValue>()->symbol());

    return value;
  }
};
}  // namespace instructions

}  // namespace wellnmea
