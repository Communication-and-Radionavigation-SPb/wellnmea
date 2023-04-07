#pragma once

#include <wellnmea/formats/lexem.hpp>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>
#include <wellnmea/formats/format_builder.hpp>

using wellnmea::formats::Format;
using wellnmea::formats::FormatBuilder;
using wellnmea::formats::Instruction;
using wellnmea::formats::Lexem;

namespace wellnmea
{

  class DefaultFormatBuilder : public FormatBuilder
  {
  public:
    DefaultFormatBuilder() = default;

  protected:
    std::shared_ptr<Instruction> onField(Lexem *lex) const override
    {
      FormatBuilder::validateFieldLexem(lex);
      auto name = lex->sublexems[0]->slice;
      auto formatter = lex->sublexems[1]->slice;

      std::list<std::string> params{};
      if (lex->sublexems.size() > 2)
      {
        for (auto &&p : lex->sublexems[2]->sublexems)
        {
          params.push_back(p->slice);
        }
      }

      auto instruction = formats::InstructionsRegistry::get(formatter)->clone(name)->applyParams(params);
      return std::shared_ptr<Instruction>(instruction);
    }

    std::shared_ptr<Instruction> onRepetition(Lexem *lex) const override
    {
      FormatBuilder::validateRepetitionLexem(lex);
      return std::shared_ptr<Instruction>(nullptr);
    }

  public:
    std::shared_ptr<Format> build(const std::vector<Lexem *> &lexems) const override
    {
      if (lexems.empty())
        throw configuration_error("Empty format configuration provided into format builder");
        
      Format::Items items{};

      for (auto it = lexems.begin(); it < lexems.end(); it++)
      {
        if ((*it)->type == Lexem::field)
        {
          auto ptr = onField(*it);
          items.push_back(ptr);
          continue;
        }
        if((*it)->type == Lexem::repetition) {
          auto ptr = onRepetition(*it);
          items.push_back(ptr);
          continue;
        }
        throw configuration_error("Invalid configuration provided. Error occured at `" +
                                  (*it)->slice + "`.");
      }

      return std::make_shared<Format>(items);
    }
  };
} // namespace wellnmea
