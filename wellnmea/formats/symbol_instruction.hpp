#pragma once

#include <string>
#include <list>
#include <iostream>
#include <wellnmea/formats/instruction.hpp>

namespace wellnmea
{
  namespace formats
  {
    template <class NullValue, class SignificantValue>
    class SymbolInstruction : public Instruction
    {
      bool m_required;
      std::list<char> m_allowed_symbols;

    public:
      SymbolInstruction(const std::string &name,
                        std::list<char> symbols = {},
                        bool required = false) : Instruction(name),
                                                 m_required(required),
                                                 m_allowed_symbols(symbols) {}

    protected:
      bool validate(value *value) const
      {
        return true;
      }

    public:
      value *extract(position it, const_position end) override
      {
        auto current_it = it++;
        if (current_it == end)
        {
          throw extraction_error("Source length failure on symbol extraction");
        }
        if (current_it->type == Token::null)
        {
          return new NullValue(name());
        }
        if (current_it->type != Token::symbol)
        {
          throw extraction_error("Failed to extract symbol near `" + current_it->slice + "`");
        }

        char ch;
        try
        {
          ch = current_it->slice.at(0);
        }
        catch (const std::exception &e)
        {
          throw extraction_error("Failed to extract symbol near `" + current_it->slice + " `: " + e.what());
        }

        auto found_it = std::find(m_allowed_symbols.begin(), m_allowed_symbols.end(), ch);

        if (found_it == m_allowed_symbols.end())
        {
          throw extraction_error("Unsupported `" + current_it->slice + "` symbol met.");
        }

        return new SignificantValue(name(), ch);
      }

      std::string which() const noexcept override
      {
        return "symbol";
      }

      SymbolInstruction *applyParams(props params) override
      {
        if (params.empty())
        {
          return this;
        }

        m_required = params[0] == "true" ? true : false;
        return this;
      }

      virtual SymbolInstruction *clone(const std::string &name) const override
      {
        return new SymbolInstruction(name, m_allowed_symbols, m_required);
      }
    };

  } // namespace formats

} // namespace wellnmea
