#pragma once

#include <stack>
#include <string>
#include <vector>

#include <wellnmea/util/string_utils.hpp>
#include <wellnmea/formats/lexem.hpp>

namespace wellnmea
{
  namespace formats
  {
    using wellnmea::util::extract_between;
    class FormatInterpreter
    {
    public:
      FormatInterpreter() = delete;

      static std::vector<Lexem *> fieldInterpret(const std::string &field)
      {
        std::vector<Lexem *> result;
        auto end = field.end();
        for (auto it = field.begin(); it != field.end(); it++)
        {

          // * If params section start sign detected
          if (*it == '(')
          {
            // Find end of params section
            auto paramsEnd = extract_between('(', ')', it, end);
            // Separate params section
            std::string paramsPart = std::string(it + 1, paramsEnd);
            // Interpret params
            auto sub = fieldInterpret(paramsPart);
            // Create params lexem and push it to results
            result.emplace_back(new Lexem(Lexem::params, paramsPart, sub));
            // Move iterator to the end of params section
            it = paramsEnd;
            continue;
          }

          // * If repetition start sign detected
          if (*it == '[')
          {
            // Find iterator at the end of repetition section
            auto repetitionEnd = extract_between('[', ']', it, end);
            // Separate repeated part
            std::string repetitionPart = std::string(it + 1, repetitionEnd);
            // Execute interpretion for repeated part
            auto sub = interpret(repetitionPart);
            // Create repetition lexem
            auto lex = new Lexem(Lexem::repetition, repetitionPart, sub);

            result.emplace_back(lex);
            // Move iterator to the end of repeated part
            it = repetitionEnd;
            continue;
          }

          if ((*it) == ',' || (*it) == '|' || std::isspace(*it))
          {
            continue;
          }

          if (std::ispunct(*it))
          {
            throw std::invalid_argument("Invalid symbol meet in field declaration");
          }

          if (std::isdigit(*it))
          {
            auto cutpos = std::find_if(it, end, [](const char &c)
                                       { return !std::isdigit(c) && c != '.'; });

            std::string slice = std::string(it, cutpos);
            result.emplace_back(new Lexem(Lexem::number, slice));
            it = --cutpos;
            continue;
          }

          // Find position where word lexem terminated
          auto cutpos = find_if(it, end, [](const char &c)
                                { return !std::isalnum(c) && c != '_'; });

          std::string slice = std::string(it, cutpos);
          it = --cutpos;

          result.emplace_back(new Lexem(Lexem::word, slice));
        }

        return result;
      }

      static std::vector<Lexem *> interpret(const std::string &source)
      {
        // Interpreted source
        std::vector<Lexem *> result;

        auto end = source.end();

        for (auto it = source.begin(); it != end; it++)
        {
          // * Skip formatting signs
          if (std::isspace(*it))
            continue;

          // Skip delimeter signs
          if (std::ispunct(*it))
            continue;

          // * Interpret field

          // Find field border
          auto fieldBorder = util::border_sign(';', {{'[', ']'}}, it, end);
          // Separate field section
          std::string field = std::string(it, fieldBorder);

          // Interpret field section
          auto sub = fieldInterpret(field);
          // Create field lexem and push it to results
          result.emplace_back(new Lexem(Lexem::field, field, sub));
          // Move iterator to the field border
          it = --fieldBorder;
        }

        return result;
      }
    };
  } // namespace formats

} // namespace wellnmea
