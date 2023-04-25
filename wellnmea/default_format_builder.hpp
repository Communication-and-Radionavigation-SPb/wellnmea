#pragma once

#include <wellnmea/formats/lexem.hpp>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/formats/repeated_instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>
#include <wellnmea/formats/format_builder.hpp>

using wellnmea::formats::Format;
using wellnmea::formats::FormatBuilder;
using wellnmea::formats::Instruction;
using wellnmea::formats::RepeatedInstruction;
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

      std::vector<std::string> params{};
      if (lex->sublexems.size() > 2)
      {
        params = std::vector<std::string>(lex->sublexems[2]->sublexems.size());
        auto it = lex->sublexems[2]->sublexems.begin();
        auto size = params.size();
        for (size_t i = 0; i < size; i++)
        {
          params[i] = (*it)->slice;
          ++it;
        }
      }

      auto instruction = formats::InstructionsRegistry::get(formatter)->clone(name)->applyParams(params);
      return std::shared_ptr<Instruction>(instruction);
    }

    std::shared_ptr<Instruction> onRepetition(Lexem *lex) const override
    {
      FormatBuilder::validateRepetitionLexem(lex);
      auto name = lex->sublexems[0]->slice;
      std::list<RepeatedInstruction::Subfield> subfields{};
      
      for (auto it = ++(lex->sublexems.begin()); it < lex->sublexems.end(); it++)
      {
        subfields.emplace_back(onField(*it));
      }

      return std::make_shared<RepeatedInstruction>(name, subfields);
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
