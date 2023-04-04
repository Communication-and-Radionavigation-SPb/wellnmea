#pragma once

#include <string>
#include <memory>
#include <list>
#include <wellnmea/exceptions.hpp>
#include <wellnmea/token.hpp>
#include <wellnmea/values/null_value.hpp>

using wellnmea::values::NullValue;

namespace wellnmea
{
  namespace formats
  {
    /**
     * @brief Base class for all NMEA message items
     *
     */
    class Instruction
    {
    public:
      /* ---------------------------------- Types --------------------------------- */

      using value = std::shared_ptr<NullValue>;
      using position = std::list<Token>::iterator&;
      using props = const std::list<std::string>;
      /* ---------------------------------- Data ---------------------------------- */
    protected:
      std::string m_name;

    public:
      Instruction(const std::string &name) : m_name(name) {}

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
       * @param it Cursor inside NMEA message represented with Tokens
       * @return value Value for this item
       */
      virtual value extract(position it) {
        return std::make_shared<values::NullValue>(name());
      };

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
      virtual Instruction *applyParams(props params) = 0;

      /**
       * @brief Clones this Instruction
       *
       * @return std::shared_ptr<Instruction> Cloned item
       */
      virtual Instruction *clone(const std::string &name) const = 0;
    };
  } // namespace formats

} // namespace welnmea
