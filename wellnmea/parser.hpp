#pragma once

#include <iostream>
#include <string>
#include <wellnmea/sentence.hpp>
#include <wellnmea/util/string_utils.hpp>
#include <wellnmea/util/number_utils.hpp>

namespace wellnmea
{

  class Parser
  {
  protected:
    void sanitize(std::string &source)
    {
      char removable[] = {'\t', ' ', '\r', '\n'};
      for (const char i : removable)
      {
        source.erase(std::remove(source.begin(), source.end(), i), source.end());
      }
    }

    uint8_t calculateChecksum(std::string_view payload)
    {
      uint8_t checksum = 0;
      for (const char c : payload)
      {
        checksum = checksum ^ c;
      }
      return checksum;
    }

  public:
    void parseInto(Sentence &sentence, std::string source)
    {
      if (source.empty())
      {
        return;
      }
      // Remove unnecessary symbols
      sanitize(source);
      // Assign
      sentence.text = source;
      // Remove dollar sign
      size_t dollarpos = source.find_last_of('$');
      // Return on no dollar sign
      if (dollarpos == std::string::npos)
      {
        return;
      }
      // Create the working string window without allocating much memory
      std::string_view workwindow{sentence.text.c_str() + dollarpos + 1, source.size()};

      // When there is no payload
      if (workwindow.empty())
      {
        return;
      }
      // Look for checksum
      size_t starsympos = workwindow.find_last_of('*');
      if (starsympos != std::string::npos)
      {

        std::string_view checksum(workwindow);
        checksum.remove_prefix(starsympos + 1);
        sentence.checksumField = checksum;
        sentence.parsedChecksum = (uint8_t)util::toInt(std::string{checksum.begin(), checksum.end()}, 16);

        workwindow.remove_suffix(workwindow.size() - starsympos);
        sentence.payloadChecksum = calculateChecksum(workwindow);
      }

      size_t commapos = workwindow.find(',');
      // Returns if name is not presented
      if (commapos == 0)
      {
        return;
      }
      // When there is no fields, but name is presented
      // Return if name is not valid
      if (commapos == std::string::npos && util::hasNonAlphaNumeric(workwindow))
      {
        return;
      }
      sentence.talker = std::string_view{sentence.text.c_str() + dollarpos + 1, 2};
      sentence.formatter = std::string_view{sentence.text.c_str() + dollarpos + 3, 3};

      // When comma is the only presented symbol in fields
      // Example: $HEHDT,*79
      if (commapos + 1 == workwindow.size())
      {
        sentence.fields.push_back(std::string_view{sentence.text.c_str(), 0});
        sentence.markValid();
        return;
      }

      // Adjust workwindow to the position of first comma
      workwindow.remove_prefix(commapos + 1);
      do
      {
        // find next comma position
        commapos = workwindow.find(',');

        if (commapos == std::string::npos)
        {
          std::string_view field(workwindow);
          sentence.fields.push_back(field);
          break;
        }

        // create field view and add it into fields
        std::string_view field(workwindow);
        field.remove_suffix(workwindow.size() - commapos);

        sentence.fields.push_back(field);
        // move left border to the position of found comma
        // excluding that comma
        workwindow.remove_prefix(commapos + 1);
      } while (commapos != std::string::npos);

      // Validate parsed fields
      bool foundInvalid = false;
      for (const std::string_view& field : sentence.fields)
      {
        foundInvalid = foundInvalid && util::hasInvalidFieldChars(field);
        if(foundInvalid) break;
      }
      if(!foundInvalid) sentence.markValid();
    }

    void parse(const std::string &source)
    {
      wellnmea::Sentence sentence;
      parseInto(sentence, source);
    }
  };

} // namespace wellnmea
