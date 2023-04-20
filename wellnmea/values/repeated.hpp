#pragma once

#include <iterator>
#include <cstddef>
#include <list>
#include <vector>
#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace values
  {
    using std::list;
    using std::optional;
    using std::vector;

    class RepeatedValue : public NullValue
    {
    public:
      using Group = vector<NullValue *>;
      using GroupList = list<Group *>;

      struct Iterator
      {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Group;
        using pointer = value_type *;
        using reference = value_type &;
        using base = GroupList::const_iterator;

        Iterator(base ptr) : m_ptr(ptr) {}

        reference operator*() const { return **m_ptr; }

        pointer operator->()
        {
          return &**m_ptr;
        }

        Iterator &operator++()
        {
          m_ptr++;
          return *this;
        }

        Iterator operator++(int)
        {
          Iterator tmp = *this;
          ++(*this);
          return tmp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_ptr != b.m_ptr; };

      private:
        base m_ptr;
      };

    protected:
      GroupList m_groups;

    public:
      RepeatedValue(const std::string &name,
                    const GroupList &values) : NullValue(name),
                                               m_groups(values)
      {
      }
      ~RepeatedValue()
      {
        auto itemdeletion = [](NullValue *ptr)
        {
          delete ptr;
        };
        for (auto &&group : m_groups)
        {
          std::for_each(group->begin(), group->end(), itemdeletion);
        }
      }

      Iterator begin() const
      {
        return Iterator(m_groups.begin());
      }

      Iterator end() const
      {
        return Iterator(m_groups.end());
      }

      int size() const
      {
        return m_groups.size();
      }

      virtual void accept(visitor_base &v) const noexcept
      {
        using value_visitor = visitor<RepeatedValue>;
        if (value_visitor *ev = dynamic_cast<value_visitor *>(&v))
          ev->visit(this);
      }
    };
  } // namespace values

} // namespace wellnmea
