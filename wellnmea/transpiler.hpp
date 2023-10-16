#pragma once

#include <memory>
#include <list>
#include <unordered_map>
#include <string>

#include <wellnmea/util/string_utils.hpp>

#include <wellnmea/exceptions.hpp>
#include <wellnmea/message.hpp>
#include <wellnmea/formats/format.hpp>

using namespace wellnmea::formats;

namespace wellnmea
{
  class Transpiler
  {
  public:
    using FmtPtr = std::shared_ptr<Format>;

  private:
    std::unordered_map<std::string, FmtPtr> m_registry;

  public:
    Transpiler() : m_registry({}) {}

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
    bool contains(const std::string &fmt_name) const noexcept
    {
      return m_registry.find(fmt_name) != m_registry.end();
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
    void connect(std::string fmt_name, FmtPtr fmt)
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
     * @return FmtPtr Shared pointer to format rules object
     */
    FmtPtr getFormat(std::string fmt_name)
    {
      if (!contains(fmt_name))
      {
        throw format_unregistered(fmt_name);
      }
      return m_registry.at(fmt_name);
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
      return dump;
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
    std::shared_ptr<Message> parse(Sentence &source)
    {
      auto formatter =
          util::to_lower(std::string{source.formatter.begin(), source.formatter.end()});

      auto fmt = getFormat(formatter);

      auto parsed = fmt->parse(source);

      auto talker = std::string{source.talker.begin(), source.talker.end()};

      return std::shared_ptr<Message>(new Message(talker, formatter, parsed));
    }
  };
} // namespace wellnmea
