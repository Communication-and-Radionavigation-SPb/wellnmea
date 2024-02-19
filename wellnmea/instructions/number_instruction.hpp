#pragma once

#include <list>
#include <optional>
#include <sstream>
#include <tuple>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>

namespace wellnmea {
namespace instructions {

class NumberValue : public NullValue {
 protected:
  std::optional<double> value_;

 public:
  NumberValue(const std::string& name) : NullValue(name) {}

 public:
  virtual void accept(visitor_base& v) const noexcept override {
    using value_visitor = visitor<NumberValue>;
    if (value_visitor* ev = dynamic_cast<value_visitor*>(&v))
      ev->visit(this);
  }

  void setValue(std::optional<double> value) { value_ = value; }

  std::optional<double> getValue() const noexcept { return value_; }

  /**
   * @brief checks if value is empty
   */
  bool empty() const noexcept { return !value_.has_value(); }
};

/* -------------------------------------------------------------------------- */
/*                             Number instruction                             */
/* -------------------------------------------------------------------------- */

class NumberInstruction : public Instruction {
 public:
  NumberInstruction(const std::string& name) : Instruction(name) {}

 public:
  virtual std::string which() const noexcept override { return "number"; }

  virtual Instruction* clone(const std::string& name) const override {
    return new NumberInstruction(name);
  }

  virtual value* extract(position it, const_position end) override {
    auto result = new NumberValue(name());

    if (!it->empty()) {
      try {
        auto num = util::toDouble(*it);
        result->setValue(num);
      } catch (const std::exception& e) {
        std::stringstream ss;
        ss << "Could not decode number field" << *it << ": " << e.what();
        throw NumberDecodeError(ss.str());
      }
    }

    ++it;
    return result;
  }
};

class RangeNumberInstruction : public NumberInstruction {
 protected:
  std::tuple<int, int> range_;

 public:
  RangeNumberInstruction(const std::string& name, std::tuple<int, int> range)
      : NumberInstruction(name), range_(range) {}

 public:
  std::string which() const noexcept override { return "range"; }

  Instruction* clone(const std::string& name) const override {
    return new RangeNumberInstruction(name, range_);
  }

  virtual value* extract(position it, const_position end) override {
    auto value = NumberInstruction::extract(it, end)->as<NumberValue>();

    if (value->empty())
      return value;

    auto digit = value->getValue().value();
    auto left = std::get<0>(range_);
    auto right = std::get<1>(range_);
    if (digit < left || digit > right) {
      std::stringstream ss;

      ss << "Number "
         << "`" << digit << "`. "
         << "must be in range"
         << " [" << left << ", " << right << "]";

      throw NumberRangeError(ss.str());
    }

    return value;
  }
};

}  // namespace instructions
}  // namespace wellnmea
