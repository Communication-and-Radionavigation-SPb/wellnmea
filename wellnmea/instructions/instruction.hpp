#pragma once

#include <string>
#include <memory>
#include <list>
#include <vector>
#include <wellnmea/exceptions.hpp>

namespace wellnmea
{
  namespace instructions
  {
    /**
     * @brief Visitor template
     * 
     * @tparam _Visitable Type which this visitor can visit
     */
    template <typename _Visitable>
    struct visitor
    {
      virtual void visit(const _Visitable *) = 0;
    };

    /**
     * @brief Base visitor
     * 
     */
    struct visitor_base
    {
      virtual ~visitor_base() {}
    };

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
      virtual std::string_view name() const noexcept
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

    /**
     * @brief Base class for sentence fields parsing instructions
     *
     */
    class Instruction
    {
    public:
      /* ---------------------------------- Types --------------------------------- */

      using value = NullValue;
      using position = std::list<std::string_view>::iterator &;
      using const_position = const std::list<std::string_view>::const_iterator &;
      using props = const std::vector<std::string>;
      /* ---------------------------------- Data ---------------------------------- */
    protected:
      std::string m_name;

    public:
      Instruction(const std::string &name) : m_name(name) {}

      virtual ~Instruction() = default;

      /* -------------------------------- Interface ------------------------------- */
    public:
      /**
       * @brief Name of current Instruction
       *
       * @return std::string Name of this Instruction
       */
      std::string name() const noexcept
      {
        return m_name;
      }

      /**
       * @brief Returns formatter of NMEA message item
       *
       * @return std::string Formatter of NMEA message item. Example: utc, longitude
       */
      virtual std::string which() const noexcept = 0;

      /**
       * @brief Extracts value of this NMEA message item
       *
       * @param it Cursor inside NMEA message represented with sentence fields
       * @return value Value for this item
       */
      virtual value *extract(position it, const_position end) = 0;

      /**
       * @brief Applies new instruction params
       *
       * Accepts bunch of new params and
       * applies them to this instruction.
       *
       * @throws wellnmea::invalid_param If one of parameters can
       * not be applied to this instruction
       * @return Current object reference
       */
      virtual Instruction *applyParams(props params)
      {
        return this;
      }

      /**
       * @brief Clones this Instruction
       *
       * @return std::shared_ptr<Instruction> Cloned item
       */
      virtual Instruction *clone(const std::string &name) const = 0;
    };
  } // namespace formats

} // namespace welnmea
