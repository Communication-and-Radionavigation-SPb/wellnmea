#pragma once

#include <algorithm>
#include <list>
#include <sstream>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>
#include <wellnmea/util/string_utils.hpp>

namespace wellnmea {
namespace instructions {

class CharacterValue : public NullValue {
 public:
  using visitor_t = visitor<CharacterValue>;

 protected:
  std::optional<char> value;

 public:
  CharacterValue(const std::string& name) : NullValue(name) {}

 public:
  virtual void accept(visitor_base& v) const noexcept override {
    visitor_t* ev = dynamic_cast<visitor_t*>(&v);
    if (ev != nullptr) {
      ev->visit(this);
    }
  }

  void setSymbol(std::optional<char> c) noexcept { value = c; }

  std::optional<char> symbol() const { return value; }
};
/* -------------------------------------------------------------------------- */
/*                                 Instruction                                */
/* -------------------------------------------------------------------------- */
class SymbolInstruction : public Instruction {
 private:
  std::list<char> allowed_chars;

 public:
  SymbolInstruction(const std::string& name, std::list<char> allowed = {})
      : Instruction(name), allowed_chars(allowed) {}

 public:
  std::string which() const noexcept override { return "char"; }

  value* extract(position it, const_position end) override {
    auto result = new CharacterValue(name());

    if (it->size() > 1 || util::hasNonAlpha(*it)) {
      std::stringstream ss;
      ss << "Character value can contain only alphabetical characters, but"
         << (*it) << " received";
      throw extraction_error(ss.str());
    }
    if (it->size() > 0) {
      char c = it->at(0);
      if (!allowed_chars.empty() &&
          std::find(allowed_chars.begin(), allowed_chars.end(), c) ==
              allowed_chars.end()) {
        std::stringstream ss;
        ss << "Disallowed character"
           << "`" << c << "` "
           << "met in field `" << (*it) << "`";
        throw extraction_error(ss.str());
      }

      result->setSymbol(c);
    }

    ++it;
    return result;
  }

  virtual Instruction* clone(const std::string& name) const override {
    return new SymbolInstruction(name);
  }
};

}  // namespace instructions

}  // namespace wellnmea
