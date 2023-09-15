#pragma once

#include <string>
#include <vector>
#include <memory>
#include <wellnmea/formats/lexem.hpp>
#include <wellnmea/formats/format.hpp>
#include <wellnmea/formats/instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>

namespace wellnmea
{
  namespace formats
  {
    /**
     * @brief Interface for
     *
     * Analyses passed lexems and produces format of NMEA message
     * which can be used further for parsing.
     */
    class FormatBuilder
    {
    public:
      FormatBuilder() = default;

    protected:
      /**
       * @brief Flow of concrete Instruction construction out from Lexem.
       *
       * Rules to construct concrete Instruction out from Lexem.
       * Base class already contains general validation
       * rules.
       * @throws configuration_error When configuration format is invalid
       * @param lex Lexem containing Instruction configuration
       * @return Format::ItemPtr Constructed Instruction pointer
       */
      virtual Format::ItemPtr onField(Lexem *lex) const = 0;
      /**
       * @brief Repeated instruction construction out from Lexem
       * 
       * Rules to construct RepeatedInstruction with
       * contained concrete instructions inside
       * 
       * @throws If provided lexem can not be considered as
       * valid repetition lexem. It could happen if `lex->sublexems.size()`
       * is less than 2. Or first sublexem is not word which represents
       * name of repeated part.
       * 
       * @param lex Lexem containing Repeated instruction configuration
       * @return Format::ItemPtr Constructed repeated instruction pointer
       */
      virtual Format::ItemPtr onRepetition(Lexem *lex) const = 0;

      virtual void validateRepetitionLexem(Lexem *lex) const
      {
        auto sub = lex->sublexems;
        if (sub.size() < 2)
          throw configuration_error("Invalid configuration provided."
                                    " Repeated part can not be empty.");
        if (sub[0]->type != Lexem::word || sub[0]->slice.empty())
          throw configuration_error("Invalid configuration provided. "
                                    "Repeated part must have name but `" +
                                    lex->slice + "` provided.");
        for (auto &&l : sub)
        {
          if (l->type != Lexem::field && l->type != Lexem::word)
          {
            throw configuration_error("Invalid configuration provided. "
                                      "Repeated part can contain only field instructions but `" +
                                      l->slice +
                                      "` provided.");
          }
        }
      }

      /**
       * @brief Validates Lexem::field type
       * @throws configuration_error If provided lexem can not be
       * considered as valid field
       * @param lex Lexem to validate
       */
      virtual void validateFieldLexem(Lexem *lex) const
      {
        auto sub = lex->sublexems;
        if (sub.size() < 2)
          throw configuration_error("Invalid configuration provided."
                                    " Minimal NMEA field rule is: `name|format`");
        if (sub.size() > 3)
          throw configuration_error("Invalid configuration provided. "
                                    "Too few statements for a field in `" +
                                    lex->slice + "`");

        if (sub[0]->type != Lexem::word)
          throw configuration_error("Invalid configuration provided."
                                    " Error occured at `" +
                                    lex->slice + "`");
        if (sub[0]->slice.empty())
          throw configuration_error("Invalid configuration provided."
                                    " Field name can not be empty but `" +
                                    lex->slice + "` provided.");

        if (sub[1]->type != Lexem::word)
          throw configuration_error("Invalid configuration provided."
                                    " Unknown field format provided in `" +
                                    lex->slice + "`.");

        if (sub[1]->slice.empty())
          throw configuration_error("Invalid configuration provided."
                                    " Field formatter can not be empty but `" +
                                    lex->slice + "` provided.");

        if (sub.size() > 2 && sub[2]->type != Lexem::params)
          throw configuration_error("Invalid configuration provided."
                                    " Invalid field params statement received in `" +
                                    lex->slice + "`.");
      }

    public:
      /**
       * @brief Creates Format from provided lexems set
       * 
       * @param lexems Sequence of lexems which represents format
       * parsing rules
       * @return std::shared_ptr<Format> Created format containing parsing instructions
       */
      virtual std::shared_ptr<Format> build(const std::vector<Lexem *> &lexems) const = 0;
    };
  } // namespace formats

} // namespace wellnmea
