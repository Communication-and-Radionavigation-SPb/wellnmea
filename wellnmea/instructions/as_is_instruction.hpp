#pragma once
#include <string>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/string_utils.hpp>

namespace wellnmea {
namespace instructions {
class AsIsValue : public NullValue {
 public:
  using visitor_t = visitor<AsIsValue>;

 protected:
  std::optional<std::string> value;

 public:
  AsIsValue(const std::string& name) : NullValue(name) {}

 public:
  virtual void accept(visitor_base& v) const noexcept override {
    visitor_t* ev = dynamic_cast<visitor_t*>(&v);
    if (ev != nullptr) {
      ev->visit(this);
    }
  }

  void set(std::optional<std::string> c) noexcept { value = c; }

  std::optional<std::string> contents() const { return value; }
};
/* -------------------------------------------------------------------------- */
/*                                 Instruction                                */
/* -------------------------------------------------------------------------- */
class AsIsInstruction : public Instruction {

 public:
  AsIsInstruction(const std::string& name) : Instruction(name) {}

 public:
  std::string which() const noexcept override { return "as_is"; }

  value* extract(position it, const_position end) override {
    auto value = new AsIsValue(name());

    if (!it->empty()) {
      value->set(std::move(std::string{it->begin(), it->end()}));
    }

    ++it;
    return value;
  }

  virtual Instruction* clone(const std::string& name) const override {
    return new AsIsInstruction(name);
  }
};
}  // namespace instructions
}  // namespace wellnmea
