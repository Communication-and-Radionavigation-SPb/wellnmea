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
      std::map<std::string,ValuePtr>
          parse(std::list<Token>::iterator start,
                 std::list<Token>::iterator end) const
      {
        return std::map<std::string,ValuePtr>();
      }

      size_t count() const {
        return m_items.size();
      }
    };
  }
} // namespace wellnmea
