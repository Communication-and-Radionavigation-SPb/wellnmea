#pragma once

#include <cstring>
#include <sstream>
#include <string>
#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>

namespace wellnmea {
namespace instructions {
/* -------------------------------------------------------------------------- */
/*                              Coordinate value                              */
/* -------------------------------------------------------------------------- */

class CoordinateValue : public NullValue {
 protected:
  std::optional<double> base;
  double degrees_ = 0;
  double minutes_ = 0;

 public:
  CoordinateValue(const std::string& name) : NullValue(name) {}

 public:
  virtual void accept(visitor_base& v) const noexcept override {
    using value_visitor = visitor<CoordinateValue>;
    if (value_visitor* ev = dynamic_cast<value_visitor*>(&v))
      ev->visit(this);
  }
  virtual void setBase(std::optional<double> value) noexcept {
    base = value;
    if (!base.has_value())
      return;

    auto uvalue = std::fabs(base.value());
    int sign = base.value() / uvalue;
    degrees_ = std::trunc(uvalue / 100);
    minutes_ = sign * (uvalue - degrees_ * 100);
    degrees_ *= sign;
  }

  virtual void reset() noexcept { base = std::nullopt; }

  virtual bool has_value() const noexcept { return base.has_value(); }

  std::optional<double> decimal() const {
    if (base.has_value()) {
      return degrees_ + minutes_ / 60.0;
    }
    return std::nullopt;
  }

  std::optional<int> degrees() const {
    if (base.has_value()) {
      return degrees_;
    }
    return std::nullopt;
  }

  std::optional<double> minutes() const {
    if (base.has_value()) {
      return minutes_;
    }
    return std::nullopt;
  }
};
/* -------------------------------------------------------------------------- */
/*                                 Instruction                                */
/* -------------------------------------------------------------------------- */
class CoordinateInstruction : public Instruction {
 public:
  CoordinateInstruction(const std::string& name) : Instruction(name) {}

 public:
  std::string which() const noexcept override { return "coordinate"; }

  value* extract(position it, const_position end) override {
    auto coordinate = new CoordinateValue(name());
    if (!it->empty()) {
      try {
        double point = util::toDouble(*it);
        coordinate->setBase(point);
      } catch (const NumberDecodeError& e) {
        std::stringstream ss;
        ss << "Could not decode coordinate from " << (*it) << " : "
           << e.message;
        throw NumberDecodeError(ss.str());
      }
    }

    ++it;
    return coordinate;
  }

  Instruction* clone(const std::string& name) const override {
    return new CoordinateInstruction(name);
  }
};

}  // namespace instructions

}  // namespace wellnmea
