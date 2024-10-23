#pragma once

#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <wellnmea/sentence.hpp>
#include <wellnmea/util/number_utils.hpp>
#include <wellnmea/util/string_utils.hpp>

namespace wellnmea {

class Parser {
 private:
  std::unordered_map<std::string, std::function<void(Sentence)>> handlingTable;
  const char removable[4] = {'\t', ' ', '\r', '\n'};

 protected:
  void sanitize(std::string& source) {
    for (const char& i : removable) {
      source.erase(std::remove(source.begin(), source.end(), i), source.end());
    }
  }

  uint8_t calculateChecksum(std::string_view payload) {
    uint8_t checksum = 0;
    for (const char c : payload) {
      checksum = checksum ^ c;
    }
    return checksum;
  }

 public:
  void parseInto(Sentence& sentence, std::string source) {
    if (source.empty()) {
      return;
    }
    // Remove unnecessary symbols
    sanitize(source);
    // Assign
    sentence.text = source;
    // Remove dollar sign
    size_t dollar_pos = source.find_last_of("$!");
    // Return on no dollar sign
    if (dollar_pos == std::string::npos) {
      return;
    }
    // Create the working string window without allocating much memory
    const char* s_contents = sentence.text.c_str() + dollar_pos + 1;
    std::string_view workwindow{s_contents, std::strlen(s_contents)};

    // When there is no payload
    if (workwindow.empty()) {
      return;
    }
    // Look for checksum
    size_t asterisk_pos = workwindow.find_last_of('*');
    if (asterisk_pos != std::string::npos) {

      std::string_view checksum(workwindow);
      checksum.remove_prefix(asterisk_pos + 1);
      sentence.checksumField = checksum;
      sentence.parsedChecksum = (uint8_t)util::toInt(
          std::string{checksum.begin(), checksum.end()}, 16);

      workwindow.remove_suffix(workwindow.size() - asterisk_pos);
      sentence.payloadChecksum = calculateChecksum(workwindow);
    }

    size_t comma_pos = workwindow.find(',');
    // Returns if name is not presented
    if (comma_pos == 0) {
      return;
    }
    // When there is no fields, but name is presented
    // Return if name is not valid
    if (comma_pos == std::string::npos &&
        util::hasNonAlphaNumeric(workwindow)) {
      return;
    }
    sentence.talker =
        std::string_view{sentence.text.c_str() + dollar_pos + 1, 2};
    sentence.formatter =
        std::string_view{sentence.text.c_str() + dollar_pos + 3, 3};

    // When comma is the only presented symbol in fields
    // Example: $HEHDT,*79
    if (comma_pos + 1 == workwindow.size()) {
      sentence.fields.push_back(std::string_view{sentence.text.c_str(), 0});
      sentence.markValid();
      return;
    }

    // Adjust workwindow to the position of first comma
    workwindow.remove_prefix(comma_pos + 1);
    do {
      // find next comma position
      comma_pos = workwindow.find(',');

      if (comma_pos == std::string::npos) {
        std::string_view field(workwindow);
        sentence.fields.push_back(field);
        break;
      }

      // create field view and add it into fields
      std::string_view field(workwindow);
      field.remove_suffix(workwindow.size() - comma_pos);

      sentence.fields.push_back(field);
      // move left border to the position of found comma
      // excluding that comma
      workwindow.remove_prefix(comma_pos + 1);
    } while (comma_pos != std::string::npos);

    // Validate parsed fields
    bool foundInvalid = false;
    for (const std::string_view& field : sentence.fields) {
      foundInvalid = foundInvalid && util::hasInvalidFieldChars(field);
      if (foundInvalid)
        break;
    }
    if (!foundInvalid)
      sentence.markValid();
  }

  void parse(const std::string& source) {
    wellnmea::Sentence sentence;
    parseInto(sentence, source);

    std::function<void(const Sentence&)> handler = handlingTable[std::string(
        sentence.formatter.begin(), sentence.formatter.end())];
    if (handler) {
      handler(sentence);
    }
  }

  void registerHandler(std::string& formatter,
                       std::function<void(const Sentence&)> handler) {
    handlingTable.erase(formatter);
    handlingTable.insert({formatter, handler});
  }
};

}  // namespace wellnmea
