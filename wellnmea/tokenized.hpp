#pragma once

#include <string>
#include <string_view>
#include <list>
#include <optional>
#include <vector>

namespace wellnmea
{

  using token = std::string_view;

  class tokenized
  {
  protected:
    /// @brief  Tokens optional list
    std::optional<std::vector<token>> m_tokens;

  public:
    /* -------------------------------------------------------------------------- */
    tokenized() : m_tokens(std::nullopt) {}
    /* -------------------------------------------------------------------------- */
    virtual ~tokenized()
    {
    }

  public:
    /// @brief Splits provided message into list of tokens
    /// @param message Message to split
    void splitTokens(std::string &message)
    {
      std::size_t last_delimeter_position = 0, index = 1; // skip $ sign
      std::list<token> tokens;
      int current_token_index = 0;
      const int mes_length = message.length();

      // Get position of first character for requested token
      while (index < mes_length)
      {
        if (message[index] == ',' || message[index] == '*' || message[index] == 0x00)
        {
          std::string_view token_view{
              &message[last_delimeter_position + 1],
              std::distance(&message[last_delimeter_position + 1], &message[index])};

          tokens.push_back(token_view);
          // Remember delimeter position
          last_delimeter_position = index;
          // Increase token index
          current_token_index++;
        }
        index++;
      }
      // Terminate if nothing have been retrieved
      if (tokens.empty())
        return;
      // Initialize tokens vector
      m_tokens = std::vector<token>({});
      // Store tokens value
      std::list<token>::iterator it = tokens.begin();
      while (it != tokens.end())
      {
        m_tokens.value().push_back(*it);
        it++;
      }
    }
    /* -------------------------------------------------------------------------- */
    std::optional<token> tokenAt(int index) const noexcept
    {
      if(!m_tokens.has_value()) return std::nullopt;
      try
      {
        return m_tokens.value().at(index);
      }
      catch(...)
      {
        return std::nullopt;
      }
      
    }
  };
} // namespace wellnmea
