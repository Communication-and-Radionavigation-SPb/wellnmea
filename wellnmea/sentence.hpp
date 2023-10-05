#pragma once

/* ----------------------------------- STL ---------------------------------- */
#include <string>
#include <optional>
#include <algorithm>
#include <vector>
#include <stdexcept>

namespace wellnmea
{
  class Sentence
  {
  private:
    bool isValid_;

  public:
    /// @brief full raw text of received message between $ and * symbols
    std::string text;
    /// @brief message talker
    std::string_view talker;
    /// @brief message formatter
    std::string_view formatter;

    /// @brief parsed checksum field content
    std::string_view checksumField;

    std::vector<std::string_view> fields;

    /// @brief calculated checksum of text property
    uint8_t payloadChecksum;
    /// @brief digit viewof checksum property
    uint8_t parsedChecksum;

  public:
    /**
     * @brief NMEA Sentence empty object
     *
     */
    Sentence() : isValid_(false), parsedChecksum(0), payloadChecksum(0) {}

  public:
    /**
     * @brief Marks this sentence as valid NMEA sentence
     */
    void markValid()
    {
      isValid_ = true;
    }

    /**
     * @brief Verifies that calculated payload checksum is equal to parsed one
     */
    bool integrity() const {
      return payloadChecksum == parsedChecksum;
    }

    bool isValid() const
    {
      return isValid_;
    }
  };
} // namespace wellnmea
