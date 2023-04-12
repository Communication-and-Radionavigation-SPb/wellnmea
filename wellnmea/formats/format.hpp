#pragma once

#include <memory>
#include <list>
#include <map>
#include <wellnmea/formats/instruction.hpp>

namespace wellnmea
{
  namespace formats
  {
    class Format
    {
    public:
      using ItemPtr = std::shared_ptr<Instruction>;
      using Items = std::list<ItemPtr>;
      using ValuePtr = std::shared_ptr<values::NullValue>;

    private:
      Items m_items;

    public:
      Format(const Items &items) : m_items(items) {}

    public:
      std::list<ValuePtr>
      parse(std::list<Token>::iterator it, std::list<Token>::iterator end) const
      {
        std::list<ValuePtr> result;
        for (auto &&instr : m_items)
        {
          auto value = instr->extract(it, end);
          result.emplace_back(value);
        }
        return result;
      }

      size_t count() const
      {
        return m_items.size();
      }
    };
  }
} // namespace wellnmea
