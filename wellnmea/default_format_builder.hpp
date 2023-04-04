#pragma once

#include <wellnmea/formats/lexem.hpp>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>
#include <wellnmea/formats/format_builder.hpp>

using wellnmea::formats::FormatBuilder;
using wellnmea::formats::Instruction;
using wellnmea::formats::Lexem;

namespace wellnmea
{

  class DefaultFormatBuilder : public FormatBuilder
  {
  public:
    DefaultFormatBuilder() = default;

  public:
    std::shared_ptr<Instruction> onField(Lexem *lex) const override
    {
      FormatBuilder::validateFieldLexem(lex);
      auto formatter = lex->sublexems[1]->slice;
      auto name = lex->sublexems[0]->slice;
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
  };
} // namespace wellnmea
