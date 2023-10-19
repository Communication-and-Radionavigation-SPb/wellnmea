#pragma once

#include <string>
#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>

namespace wellnmea {
namespace instructions {
/* -------------------------------------------------------------------------- */
/*                                 Date value                                 */
/* -------------------------------------------------------------------------- */
class DateValue : public NullValue {
 protected:
  std::optional<int32_t> day_;
  std::optional<int32_t> month_;
  std::optional<int32_t> year_;

 public:
  DateValue(const std::string& name) : NullValue(name) {}

 public:
  void set(std::optional<int32_t> day, std::optional<int32_t> month,
           std::optional<int32_t> year) {
    if (day.has_value() && month.has_value() && year.has_value()) {
      day_ = day;
      month_ = month;
      year_ = year;
      return;
    }

    day = month = year = std::nullopt;
  }

  bool has_value() const noexcept {
    return day_.has_value() && month_.has_value() && year_.has_value();
  }

  std::optional<int32_t> day() const noexcept { return day_; }

  std::optional<int32_t> month() const noexcept { return month_; }

  std::optional<int32_t> year() const noexcept { return year_; }
};
/* -------------------------------------------------------------------------- */
/*                              Date instruction                              */
/* -------------------------------------------------------------------------- */

class DateInstruction : public Instruction {
 public:
  DateInstruction(const std::string& name) : Instruction(name) {}

 public:
  std::string which() const noexcept override { return "date"; }

  Instruction* clone(const std::string& name) const override {
    return new DateInstruction(name);
  }

 public:
  value* extract(position it, const_position end) override {
    auto value = new DateValue(name());

    if (!it->empty()) {
      auto field = std::string(it->begin(), it->end());
      auto raw_date = util::toInt(field, 10);
      int32_t day = (int32_t)trunc(raw_date / 10000.0);
      int32_t month = (int32_t)trunc((raw_date - day * 10000) / 100.0);
      int32_t year = raw_date - 10000 * day - 100 * month;

      value->set(day, month, year);
    }

    ++it;
    return value;
  }
};
}  // namespace instructions

}  // namespace wellnmea
