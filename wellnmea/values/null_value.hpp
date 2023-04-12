#pragma once

#include <string>
#include <wellnmea/types_def.hpp>
#include <wellnmea/values/value_visitor.hpp>

namespace wellnmea
{
  namespace values
  {
    class NullValue
    {
    private:
      std::string m_name;

    protected:
      /**
       * @brief Construct a new BaseValue object
       *
       * @param a_name Name of parameter
       */
      NullValue(const std::string &a_name) : m_name(a_name){};

    public:
      virtual ~NullValue() = default;

    public:
      /**
       * @brief Name of parameter
       *
       * @return std::string_view
       */
      std::string_view name() const noexcept
      {
        return m_name;
      }

      virtual void accept(visitor_base &v) const noexcept
      {
        using value_visitor = visitor<NullValue>;
        if (value_visitor *ev = dynamic_cast<value_visitor *>(&v))
          ev->visit(this);
      }

      template <class ConsiderAs>
      ConsiderAs *as()
      {
        return dynamic_cast<ConsiderAs *>(this);
      }
      /**
       * @brief Dynamic casting to consider object
       * as desired type
       *
       * @tparam ConsiderAs Type to cast this object to
       * @return ConsiderAs* Casted object pointer with ConsiderAs type
       */
      template <class ConsiderAs>
      const ConsiderAs *as() const
      {
        return dynamic_cast<const ConsiderAs *>(this);
      }
    };

  } // namespace params

} // namespace wellnmea
