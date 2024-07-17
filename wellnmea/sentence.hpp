#pragma once

/* ----------------------------------- STL ---------------------------------- */
#include <algorithm>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>

namespace wellnmea {
class Sentence {
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

  /// @brief List of parsed fields
  std::list<std::string_view>
      fields;  // list + string_view choosed by results of benchmark/list_to_vector

  /// @brief calculated checksum of text property
  uint8_t payloadChecksum;
  /// @brief digit viewof checksum property
  uint8_t parsedChecksum;

 public:
  /**
     * @brief NMEA Sentence empty object
     *
     */
  Sentence() : isValid_(false), payloadChecksum(0), parsedChecksum(0) {}

 public:
  /**
     * @brief Marks this sentence as valid NMEA sentence
     */
  void markValid() { isValid_ = true; }

  /**
     * @brief Verifies that calculated payload checksum is equal to parsed one
     */
  bool integrity() const { return payloadChecksum == parsedChecksum; }

  bool isValid() const { return isValid_; }
};
}  // namespace wellnmea
