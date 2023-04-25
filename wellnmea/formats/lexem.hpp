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
      Lexem(Type a_type,
            const std::string &a_slice) : type(a_type),
                                        slice(a_slice),
                                        sublexems({}) {}
      Lexem(Type a_type,
            const std::string &a_slice,
            std::vector<Lexem *> &a_sub) : type(a_type),
                                         slice(a_slice),
                                         sublexems(a_sub) {}

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
