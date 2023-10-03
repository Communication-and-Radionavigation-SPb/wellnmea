#pragma once

#include <string>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/number.hpp>

namespace wellnmea
{
  namespace formats
  {
    class NumberInstruction : public Instruction
    {
      bool m_required;

    public:
      NumberInstruction(const std::string &name, bool required = false) : Instruction(name),
                                                                          m_required(required)
      {
      }

    protected:
      bool validate(position it, const_position end) const
      {
        if (it != end && !it->slice.empty())
        {
          return true;
        }

        return m_required && (throw extraction_error("Number field marked as required but"
                                                     " provided message contains empty slot."),
                              false);
      }

    public:
      value *extract(position it, const_position end) override
      {
        if (!validate(it, end))
        {
          return new values::NullNumberValue(name());
        }
        auto current_it = it++;

        if (current_it->type == Token::null)
        {
          return new values::NullNumberValue(name());
        }
        if (current_it->type != Token::number)
          throw extraction_error("Failed to extract number near `" + it->slice + "`");

        double value = std::stod(current_it->slice);

        return new values::NumberValue(name(), value);
      }

      virtual std::string which() const noexcept override
      {
        return "number";
      }

      NumberInstruction *applyParams(props params) override
      {
        if (params.empty())
        {
          return this;
        }

        m_required = params[0] == "true" ? true : false;
        return this;
      }

      virtual NumberInstruction *clone(const std::string &name) const override
      {
        return new NumberInstruction(name, m_required);
      }
    };
  } // namespace formats

} // namespace wellnmea
