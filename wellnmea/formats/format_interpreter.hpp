#pragma once

#include <stack>
#include <string>
#include <vector>
#include <wellnmea/exceptions.hpp>
#include <wellnmea/util/string_utils.hpp>
#include <wellnmea/formats/lexem.hpp>

namespace wellnmea
{
  namespace formats
  {
    using wellnmea::util::extract_between;
    using wellnmea::util::punct_position;
    class FormatInterpreter
    {
    public:
      FormatInterpreter() = delete;

      static std::vector<Lexem *> commonInterpret(const std::string &part)
      {
        std::vector<Lexem *> result;
        auto end = part.end();
        for (auto it = part.begin(); it < end; it++)
        {
          if ((*it) == ',' || (*it) == '|' || std::isspace(*it))
          {
            continue;
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
          if (std::isalnum(*it))
          {

            // Find position where word lexem terminated
            auto cutpos = find_if(it, end, [](const char &c)
                                  { return !std::isalnum(c) && c != '_'; });

            std::string slice = std::string(it, cutpos);
            result.emplace_back(new Lexem(Lexem::word, slice));
            it = --cutpos;
            continue;
          }
          throw std::invalid_argument("Invalid symbol meet in field declaration");
        }
        return result;
      }

      static Lexem *partInterpret(const std::string &part)
      {
        std::vector<Lexem *> result;
        Lexem::Type type = Lexem::field;

        auto end = part.end();

        for (auto it = part.begin(); it != part.end(); it++)
        {
          // * If params section start sign detected
          if (*it == '(')
          {
            // Find end of params section
            auto paramsEnd = extract_between('(', ')', it, end);
            // Separate params section
            std::string paramsPart = std::string(it + 1, paramsEnd);
            // Interpret params
            auto sub = commonInterpret(paramsPart);
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
            // Switch type of lexem
            // auto lex = new Lexem(Lexem::repetition, repetitionPart, sub);
            type = Lexem::repetition;
            // Add lexem to local result
            result.insert(result.end(), sub.begin(), sub.end());
            // Move iterator to the end of repeated part
            it = repetitionEnd;
            continue;
          }
          if (std::isalnum(*it) || *it == '+' || *it == '-')
          {
            // Find position where next part starts
            auto nextLexemPos = punct_position({'[', '('}, it, end);
            // Separate common part
            std::string commonPart = std::string(it, nextLexemPos);
            // Execute interpretion for common part
            auto sub = commonInterpret(commonPart);
            // Add lexems to local result
            result.insert(result.end(), sub.begin(), sub.end());
            // Move iterator to the end of common part
            it = --nextLexemPos;
            continue;
          }
          throw std::invalid_argument("Invalid symbol meet at start of token near `" + part + "`");
        }

        return new Lexem(type, part, result);
      }

      static std::vector<Lexem *> interpret(const std::string &source)
      {
        // Interpreted source
        std::vector<Lexem *> result;

        auto end = source.end();

        for (auto it = source.begin(); it != end; it++)
        {
          // Skip border sign
          if (*it == ';')
            continue;
          // * Skip formatting signs
          if (std::isspace(*it))
            continue;

          // Find field border with uninterested matches in mind
          auto partBorder = util::border_sign(';', {{'[', ']'}, {'(', ')'}}, it, end);
          // Separate field section
          std::string part = std::string(it, partBorder);

          // Interpret field section
          auto sub = partInterpret(part);
          // Create field lexem and push it to results
          result.emplace_back(sub);
          // Move iterator to the field border
          it = --partBorder;
        }

        return result;
      }
    };
  } // namespace formats

} // namespace wellnmea
