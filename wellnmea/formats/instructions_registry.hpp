#pragma once

#include <unordered_map>
#include <string>

#include <wellnmea/exceptions.hpp>
#include <wellnmea/formats/instruction.hpp>

namespace wellnmea
{
  namespace formats
  {
    class InstructionsRegistry
    {
      using record_item = std::shared_ptr<Instruction>;

    public:
      InstructionsRegistry() = delete;

    private:
      inline static std::unordered_map<std::string, record_item> m_registry;

    public:
      /**
       * @brief Drops all registered formats
       */
      static void clear()
      {
        m_registry.clear();
      }

      /**
       * @brief Adds new NMEA message item into registry
       * and makes it available for further
       *
       * @param item
       */
      static void add(record_item item)
      {
        if (contains(item->which()))
        {
          return;
        }

        m_registry.emplace(item->which(), item);
      }

      /**
       * @brief Checks if registry contains Instruction with provided formatter
       *
       * @param item_type
       * @return true
       * @return false
       */
      static bool contains(const std::string &formatter)
      {
        return m_registry.find(formatter) != m_registry.end();
      }

      /**
       * @brief Extracts NMEA message item for provided formatter
       *
       * @throws wellnmea::configuration_error If there is no registered
       * Instruction matching provided formatter
       *
       * @param formatter Formatter of NMEA message Instruction
       * @return record_item Instruction matching provided formatter
       */
      static record_item get(const std::string &formatter)
      {
        if (!contains(formatter))
        {
          throw configuration_error("Failed to find NMEA item which"
                                    "matches provided formatter `" +
                                    formatter + "`."
                                                "Have you registered that?");
        }
        return m_registry.find(formatter)->second;
      }
    };
  } // namespace formats

} // namespace wellnmea
