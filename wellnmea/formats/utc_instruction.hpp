#pragma once

#include <cctype>
#include <string>
#include <math.h>

#include <wellnmea/exceptions.hpp>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/values/utc.hpp>

namespace wellnmea
{
  namespace formats
  {
    class UTCInstruction : public Instruction
    {
    public:
      UTCInstruction(const std::string &name) : Instruction(name) {}

    public:
      virtual std::string which() const noexcept override
      {
        return "utc";
      }

      virtual Instruction *clone(const std::string &name) const override
      {
        return new UTCInstruction(name);
      }

      virtual value *extract(position it, const_position end) override
      {
        if(it->type == Token::null) {
          return new values::NullUTCValue(name());
        }
        if (it->type != Token::number)
          throw extraction_error("Failed to extract UTC field near `" + it->slice + "`");

        double content = std::stod(it->slice);
        it++;

        int hours = (int)(content / pow(10, 4)) % 100;
        int minutes = (int)(content / pow(10, 2)) % 100;
        int seconds = (int)content % 100;
        int milliseconds = (int)(content * 1000) % 1000;

        auto v = new values::UTCValue(name(),
                                      hours, minutes,
                                      seconds, milliseconds);
        return v;
      }
    };
  } // namespace formats

} // namespace wellnmea
