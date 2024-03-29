#pragma once

#include <list>
#include <vector>
#include <wellnmea/instructions/instruction.hpp>

namespace wellnmea {
namespace instructions {
using std::list;
using std::vector;

class RepeatedValue : public NullValue {
 public:
  using Group = vector<NullValue*>;
  using GroupList = list<Group*>;

  struct Iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Group;
    using pointer = value_type*;
    using reference = value_type&;
    using base = GroupList::const_iterator;

    Iterator(base ptr) : m_ptr(ptr) {}

    reference operator*() const { return **m_ptr; }

    pointer operator->() { return &**m_ptr; }

    Iterator& operator++() {
      m_ptr++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const Iterator& a, const Iterator& b) {
      return a.m_ptr == b.m_ptr;
    };
    friend bool operator!=(const Iterator& a, const Iterator& b) {
      return a.m_ptr != b.m_ptr;
    };

   private:
    base m_ptr;
  };

 protected:
  GroupList m_groups;

 public:
  RepeatedValue(const std::string& name, const GroupList& values)
      : NullValue(name), m_groups(values) {}
  ~RepeatedValue() {
    auto itemdeletion = [](NullValue* ptr) {
      delete ptr;
    };
    for (auto&& group : m_groups) {
      std::for_each(group->begin(), group->end(), itemdeletion);
    }
  }

  Iterator begin() const { return Iterator(m_groups.begin()); }

  Iterator end() const { return Iterator(m_groups.end()); }

  int size() const { return m_groups.size(); }

  virtual void accept(visitor_base& v) const noexcept {
    using value_visitor = visitor<RepeatedValue>;
    if (value_visitor* ev = dynamic_cast<value_visitor*>(&v))
      ev->visit(this);
  }
};

/**
 * @brief Internal instruction for variadic part of NMEA message
 *
 * Internal instruction for variadic part of NMEA message
 * should not be registered.
 *
 */
class RepeatedInstruction : public Instruction {
 public:
  using subfield = Instruction*;
  using collection_t = list<subfield>;

 private:
  collection_t m_subfields;

 public:
  RepeatedInstruction(const std::string& name, const collection_t& subfields)
      : Instruction(name), m_subfields(subfields) {}

  ~RepeatedInstruction() {
    for (auto&& i : m_subfields) {
      delete i;
    }
  }

 public:
  std::string which() const noexcept override { return "repetition"; }

  Instruction* clone(const std::string& name) const override {
    collection_t cloned;

    std::transform(
        m_subfields.begin(), m_subfields.end(), std::back_inserter(cloned),
        [](const subfield& s) { return subfield(s->clone(s->name())); });

    return new RepeatedInstruction(name, cloned);
  }

  value* extract(position it, const_position end) override {
    RepeatedValue::GroupList l{};

    while (it != end) {
      subextract(it, end, l);
    }

    return new RepeatedValue(name(), l);
    return nullptr;
  }

 protected:
  void subextract(position it, const_position end,
                  RepeatedValue::GroupList& l) {
    RepeatedValue::Group* g = new RepeatedValue::Group;
    NullValue* v = nullptr;
    for (auto&& i : m_subfields) {
      v = i->extract(it, end);
      if (v == nullptr)
        continue;
      g->push_back(v);
    }
    l.push_back(g);
  }
};
}  // namespace instructions

}  // namespace wellnmea
