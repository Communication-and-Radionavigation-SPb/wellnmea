#pragma once

#include <list>
#include <string>

#include <wellnmea/lexing_iface.hpp>
#include <wellnmea/token.hpp>
#include <wellnmea/exceptions.hpp>
#include <wellnmea/message.hpp>
#include <wellnmea/formats/format_builder.hpp>
#include <wellnmea/formats/formats_registry.hpp>

using namespace wellnmea::formats;

namespace wellnmea
{
  class Parser
  {
    using lexing_ptr = std::shared_ptr<LexingIface>;
    lexing_ptr m_lexing;
    std::list<std::string> m_formats;

  public:
    Parser(
        lexing_ptr lexer,
        const std::list<std::string> formats) : m_lexing(lexer),
                                                m_formats(formats) {}

  public:
    /**
     * @brief Parses provided source NMEA0183 string
     *
     * @throws parse_error In case if provided source string
     * does not have NMEA0183 format
     * @param source Source string containing
     * @return NmeaMessage Parsed message
     */
    std::shared_ptr<Message> parse(const std::string &source)
    {
      auto tokens = m_lexing->splitTokens(source);
      auto it = tokens.begin();

      if (tokens.begin()->type != Token::phrase)
      {
        throw parse_error("Talker and formatter are apsent in message.");
      }

      auto talker = tokens.begin()->slice.substr(0, 2);
      auto formatter = tokens.begin()->slice.substr(2, 3);

      auto findit = std::find(m_formats.begin(), m_formats.end(), formatter);

      if(findit == m_formats.end()) {
        throw parse_error("Format of type " + formatter + " is unsupported by this parser instance");   
      }

      auto format = FormatRegistry::getFormat(formatter);

      auto parsed = format->parse(++it, tokens.end());


      auto msg = std::make_shared<Message>(talker, formatter, parsed);

      return msg;
    }
  };
} // namespace wellnmea
