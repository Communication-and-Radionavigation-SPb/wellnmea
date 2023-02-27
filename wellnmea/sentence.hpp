#pragma once
#include "field.hpp"
#include <string>

namespace wellnmea
{
  class sentence {
    public:
      sentence(const char*);
    public:
    /// @brief Computes checksum of sentence
    /// @return Sentence checksum
    uint8_t checksum() const noexcept;

    /// @brief Adds field to the sentence for parsing/producing messages
    void addField(const wellnmea::field &field) noexcept;

  };
  
} // namespace wellnmea
