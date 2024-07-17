#pragma once

#include <list>
#include <cassert>
#include <map>
#include <memory>
#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/sentence.hpp>

namespace wellnmea {
namespace formats {
using namespace wellnmea::instructions;
class Format {
 public:
  using ItemPtr = Instruction*;
  using Items = std::list<ItemPtr>;
  using ValuePtr = std::shared_ptr<NullValue>;

 private:
  Items m_items;

 public:
  Format(const Items& items) : m_items(items) { assert(!m_items.empty()); }

  ~Format() {
    if (m_items.size() == 0)
      return;

    for (ItemPtr item : m_items) {
      delete item;
    }
  }

 public:
  virtual std::list<ValuePtr> parse(Sentence& s) const {
    std::list<ValuePtr> result;

    auto it = s.fields.begin();
    auto end = s.fields.end();

    for (auto&& instr : m_items) {
      auto value = instr->extract(it, end);
      if (value == nullptr)
        continue;
      result.emplace_back(value);
    }
    return result;
  }

  size_t count() const { return m_items.size(); }
};
}  // namespace formats

using FmtValue = formats::Format::ValuePtr;
using FmtValues = std::list<formats::Format::ValuePtr>;
}  // namespace wellnmea
