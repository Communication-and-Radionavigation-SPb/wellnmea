#pragma once

#include <iterator>
#include <cstddef>
#include <list>
#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace values
  {
    using std::list;
    using std::optional;

    class RepeatedValue : public NullValue
    {
    public:
      using Subvalues = list<NullValue *>;
      struct Iterator
      {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = NullValue;
        using pointer = value_type *;
        using reference = value_type &;
        using base = Subvalues::iterator;

        Iterator(base ptr) : m_ptr(ptr) {}

        reference operator*() const { return **m_ptr; }

        pointer operator->() { return &**m_ptr; }

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
      Subvalues m_values;

    public:
      RepeatedValue(const std::string &name,
                    const Subvalues &values) : m_values(values),
                                               NullValue(name) {}
      ~RepeatedValue()
      {
        std::for_each(
            m_values.begin(),
            m_values.end(),
            [](NullValue *ptr)
            {
              delete ptr;
            });
      }

      Iterator begin()
      {
        return Iterator(m_values.begin());
      }

      Iterator end()
      {
        return Iterator(m_values.end());
      }

      int size()
      {
        return m_values.size();
      }

      virtual SerializedResult serialise() const noexcept override
      {
        return std::nullopt;
      }
    };
  } // namespace values

} // namespace wellnmea
