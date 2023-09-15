#pragma once

#include <list>
#include <unordered_map>
#include <string>

#include <wellnmea/util/string_utils.hpp>

#include <wellnmea/lexing_iface.hpp>
#include <wellnmea/token.hpp>
#include <wellnmea/exceptions.hpp>
#include <wellnmea/message.hpp>
#include <wellnmea/formats/format.hpp>
#include <wellnmea/formats/format_builder.hpp>

using namespace wellnmea::formats;

namespace wellnmea
{
  class Parser
  {
  public:
    using lexing_ptr = std::shared_ptr<LexingIface>;
    using fmt_record = std::shared_ptr<Format>;

  private:
    lexing_ptr m_lexing;
    std::unordered_map<std::string, fmt_record> m_registry;

  public:
    Parser(
        lexing_ptr lexer) : m_lexing(lexer),
                            m_registry({}) {}

  public:
    /**
     * @brief Drops all connected formats
     */
    void clear()
    {
      m_registry.clear();
    }
    /* -------------------------------------------------------------------------- */
    /**
     * @brief Checks if parser has format with fmt_name
     * connected
     *
     * @param fmt_name format name to check for existance
     * @return true If parsed has registered format
     * @return false otherwise
     */
    bool contains(std::string fmt_name) const noexcept
    {
      return m_registry.find(util::to_lower(fmt_name)) != m_registry.end();
    }
    /* -------------------------------------------------------------------------- */
    /**
     * @brief Connects parser with format which has fmt_name
     * to be used for further parsing
     *
     * @throws format_duplication If parser alrady contains format
     * with passed fmt_name
     *
     * @param fmt_name Associative name of format. This should match a FORMATTER
     * part of NMEA message
     * @param fmt Format object
     */
    void connect(std::string fmt_name, fmt_record fmt)
    {
      if (contains(fmt_name))
      {
        throw format_duplication(fmt_name);
      }
      auto key = util::to_lower(fmt_name);
      m_registry.emplace(key, fmt);
    }
    /* -------------------------------------------------------------------------- */
    /**
     * @brief Returns format associated with provided format name
     *
     * Returns format rules for format name.
     *
     * @param fmt_name Name of format
     * @return fmt_record Shared pointer to format rules object
     */
    fmt_record getFormat(std::string fmt_name)
    {
      if (!contains(fmt_name))
      {
        throw format_unregistered(fmt_name);
      }
      return m_registry.find(util::to_lower(fmt_name))->second;
    }

    /* -------------------------------------------------------------------------- */
    /**
     * @brief Removes format from parser
     *
     * @param fmt_name Format name to be removed
     */
    void dropFormat(std::string fmt_name) noexcept
    {
      if (!contains(fmt_name))
        return;
      m_registry.erase(fmt_name);
    }
    /* -------------------------------------------------------------------------- */
    /**
     * @brief Dumps names of connected formats
     * 
     * Returns connected format names joined with "|" characted
     * 
     * Example:
     * "hdt|gpg"
     * 
     * @return std::string format names joined with "|"
     */
    std::string dumpRegistered() noexcept
    {
      std::string dump = "";
      for (auto it = m_registry.begin(); it != m_registry.end(); it++)
      {
        dump += it->first;
        dump += "|";
      }
      return std::move(dump);
    }

    /* -------------------------------------------------------------------------- */
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

      auto format = this->getFormat(formatter);

      try
      {
        auto parsed = format->parse(++it, tokens.end());
        auto msg = std::make_shared<Message>(talker, formatter, parsed);
        return msg;
      }
      catch (const std::exception &e)
      {
        throw parse_error("Failed to parse message `" + source + "` with format `" + formatter + "`:" + e.what());
      }
    }
  };
} // namespace wellnmea
