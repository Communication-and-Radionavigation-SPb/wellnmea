#pragma once

#include <string>

#include <wellnmea/exceptions.hpp>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/degrees.hpp>

namespace wellnmea
{
  namespace formats
  {

    class DegreesInstruction : public Instruction
    {
    public:
      DegreesInstruction(const std::string &name) : Instruction(name) {}

    public:
      std::string which() const noexcept override
      {
        return "degrees";
      }

      Instruction *clone(const std::string &name) const override
      {
        return new DegreesInstruction(name);
      }

      value *extract(position it, const_position end) override
      {
        auto dit = it++, cit = it++;
        if (dit->type == Token::null || cit->type == Token::null)
          return new values::NullDegreesValue(name());
        if (dit->type != Token::number || cit->type != Token::symbol)
          throw extraction_error("Failed to extract degrees value near `" + it->slice + "`");

        double cursor = std::stod(dit->slice);
        values::DegreesValue::Measure measure = values::DegreesValue::typeFrom(cit->slice[0]);

        return new values::DegreesValue(name(), cursor, measure);
      }
    };
  } // namespace formats

} // namespace wellnmea