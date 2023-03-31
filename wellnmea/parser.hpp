#pragma once

#include <list>
#include <string>

#include <wellnmea/lexing_iface.hpp>
#include <wellnmea/token.hpp>
#include <wellnmea/exceptions.hpp>
#include <wellnmea/message.hpp>
#include <wellnmea/formats_registry.hpp>

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
    NmeaMessage parse(const std::string &source)
    {
      auto tokens = m_lexing->splitTokens(source);
      auto it = tokens.begin();

      if (tokens.begin()->type != Token::phrase)
      {
        throw parse_error("Talker and formatter are apsent in message.");
      }

      auto talker = tokens.begin()->slice.substr(0, 2);
      auto formatter = tokens.begin()->slice.substr(2, 3);

      auto format = fmt_registry::getFormat(formatter);

      if (tokens.end()->type == Token::checksum)
      {
      }

      auto msg = NmeaMessage();

      msg.formatter = formatter;
      msg.talker = talker;

      return NmeaMessage();
    }
  };
} // namespace wellnmea
