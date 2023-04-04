#pragma once

#include <vector>
#include <string>

namespace wellnmea
{
  namespace formats
  {
    /**
     * @brief Object representing part of configuration statement
     * of specific NMEA0183 message.
     *
     */
    class Lexem
    {
    public:
      enum Type
      {
        word,
        number,
        params,
        repetition,
        field,
      };

    public:
      Type type;
      std::string slice;
      std::vector<Lexem *> sublexems;

    public:
      Lexem(Type type,
            const std::string &slice) : type(type),
                                        slice(slice),
                                        sublexems({}) {}
      Lexem(Type type,
            const std::string &slice,
            std::vector<Lexem *> &sub) : type(type),
                                         slice(slice),
                                         sublexems(sub) {}

      ~Lexem()
      {
        for (auto &&lex : sublexems)
        {
          delete lex;
          lex = nullptr;
        }
      }

      operator std::string()
      {
        return std::string("Lexem(" + std::to_string(type) + ")::`" + slice + "`");
      }
    };
  } // namespace formats

} // namespace wellnmea
