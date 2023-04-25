#pragma once

#include <map>
#include <string>
#include <memory>
#include <wellnmea/exceptions.hpp>
#include <wellnmea/formats/format.hpp>

#include <wellnmea/util/string_utils.hpp>

namespace wellnmea
{
  namespace formats
  {

    class FormatRegistry
    {
      using reg_record = std::shared_ptr<Format>;

    private:
      inline static std::map<std::string, reg_record> m_registry;

    public:
      FormatRegistry() = delete;

      /**
       * @brief Drops all registered formats
       *
       */
      static void clear()
      {
        m_registry.clear();
      }

      /**
       * @brief Registers new format with formatName to be used in parser
       *
       * @throws format_duplication If registry already have registered
       * format with provided formatName
       * @param formatName Assiciative name of format
       * @param fmt Format rules
       */
      static void connect(std::string fmt_name, reg_record fmt)
      {
        if (contains(fmt_name))
        {
          throw format_duplication(fmt_name);
        }

        m_registry.emplace(util::to_lower(fmt_name), fmt);
      }

      static bool contains(std::string fmt_name) noexcept
      {
        return m_registry.find(util::to_lower(fmt_name)) != m_registry.end();
      }

      /**
       * @brief Returns format for requested NMEA fmt_name
       *
       * Returns format rules for format name.
       *
       * @param fmt_name Name of NMEA Format
       * @return format* NMEA Format rules object pointer
       */
      static reg_record getFormat(std::string fmt_name)
      {
        if (!contains(fmt_name))
        {
          throw format_unregistered(fmt_name);
        }

        return m_registry.find(util::to_lower(fmt_name))->second;
      }

      static string dumpRegistered() noexcept {
        string retval = "";
        for (auto it = m_registry.begin(); it != m_registry.end(); it++)
        {
          retval += it->first;
          retval += "|";
        }
        retval = retval.substr(0, retval.size() - 1);
        
        return retval;
      }
    };
  }
} // namespace wellnmea
