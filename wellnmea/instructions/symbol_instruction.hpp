#pragma once

#include <list>
#include <algorithm>

#include <wellnmea/instructions/instruction.hpp>
#include <wellnmea/util/number_utils.hpp>
#include <wellnmea/util/string_utils.hpp>

namespace wellnmea
{
  namespace instructions
  {

    class CharacterValue : public NullValue
    {
    public:
      using visitor_t = visitor<CharacterValue>;

    public:
      std::optional<char> value;

    public:
      CharacterValue(const std::string &name) : NullValue(name) {}

    public:
      virtual void accept(visitor_base &v) const noexcept override
      {
        visitor_t *ev = dynamic_cast<visitor_t *>(&v);
        if (ev != nullptr)
        {
          ev->visit(this);
        }
      }
    };
    /* -------------------------------------------------------------------------- */
    /*                                 Instruction                                */
    /* -------------------------------------------------------------------------- */
    class SymbolInstruction : public Instruction
    {
    private:
      std::list<char> allowed_chars;

    public:
      SymbolInstruction(const std::string &name, std::list<char> allowed = {}) : Instruction(name), allowed_chars(allowed) {}

    public:
      std::string which() const noexcept override
      {
        return "char";
      }

      value *extract(position it, const_position end) override
      {
        auto value = new CharacterValue(name());

        if (it->size() > 1 || util::hasNonAlpha(*it))
        {
          throw extraction_error("Character value can contain only alphabetical characters");
        }
        if (it->size() == 0)
        {
          return value;
        }
        char c = it->at(0);

        if (!allowed_chars.empty() && std::find(allowed_chars.begin(), allowed_chars.end(), c) == allowed_chars.end())
        {
          throw extraction_error("Disallowed character met in sentence");
        }

        value->value = c;

        it++;
        return value;
      }

      virtual Instruction *clone(const std::string &name) const override
      {
        return new SymbolInstruction(name);
      }
    };

  } // namespace instructions

} // namespace wellnmea
