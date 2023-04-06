#pragma once

#include <list>

#include <string>

#include <wellnmea/lexing_iface.hpp>
#include <wellnmea/token.hpp>
#include <wellnmea/exceptions.hpp>

namespace wellnmea
{

  /**
   * @brief Base type detector state
   *
   */
  class State
  {
  public:
    State() : m_previous(Token::null) {}
    State(Token::Type previous) : m_previous(previous) {}

    virtual ~State(){};

    virtual State *on_char(const char &c) = 0;

    virtual Token::Type current() const = 0;

  protected:
    Token::Type m_previous;
  };
  class PhraseState : public State
  {
  public:
    PhraseState() : State() {}
    PhraseState(Token::Type previous) : State(previous) {}
    ~PhraseState() {}
    Token::Type current() const override { return Token::phrase; }

    State *on_char(const char &c) override
    {
      if (!std::isalnum(c))
      {
        throw parse_error("Invalid symbol in phrase of message found.");
      }
      return new PhraseState(current());
    }
  };

  /**
   * @brief Number character type detector state
   *
   */
  class NumberState : public State
  {
  public:
    NumberState() : State() {}
    NumberState(Token::Type previous) : State(previous) {}
    ~NumberState() {}
    Token::Type current() const override { return Token::number; }

    State *on_char(const char &c) override
    {
      if (std::isdigit(c) || c == '.')
      {
        return new NumberState(current());
      }
      if (!std::isalnum(c))
      {
        throw parse_error("Invalid symbol in number of message found.");
      }
      return new PhraseState(current());
    }
  };

  class ChecksumState : public State
  {
  public:
    ChecksumState() : State() {}
    ChecksumState(Token::Type previous) : State(previous) {}
    ~ChecksumState() {}
    Token::Type current() const override { return Token::checksum; }

    State *on_char(const char &c) override
    {
      // In case if transition have been invalid
      if (m_previous != Token::null && m_previous != Token::checksum)
        throw parse_error("Invalid state transition into Checksum state");

      // Check symbol matches checksum grammar
      if (!std::isxdigit(c))
        throw parse_error("Invalid symbol in checksum of message found.");

      // Loop back into current state
      return new ChecksumState(current());
    }
  };

  class SymbolState : public State
  {
  public:
    SymbolState() : State() {}
    SymbolState(Token::Type previous) : State(previous) {}
    ~SymbolState() {}
    Token::Type current() const override { return Token::symbol; }

    State *on_char(const char &c) override
    {
      if (!std::isalnum(c) && c != '.' && m_previous != Token::null)
        throw parse_error("Invalid symbol got from nmea message");
      return new PhraseState(current());
    }
  };
  /**
   * @brief Initial type detector state
   *
   */
  class NullState : public State
  {
  protected:
  public:
    NullState() : State() {}
    NullState(Token::Type previous) : State(previous) {}
    ~NullState() {}

    Token::Type current() const override { return Token::null; }

    State *on_char(const char &c) override
    {
      // In case if transition have been invalid
      if (m_previous != Token::null)
        throw parse_error("Invalid null-state transition detected.");
      if (!std::isalnum(c) && c != '*' && c != '-')
      {
        throw parse_error("Invalid starting symbold detected in nmea message field");
      }
      // If digit found
      if (std::isdigit(c) || c == '-' || c == '+')
        return new NumberState(current());

      // Potential transition into checksum state
      if (c == '*')
        return new ChecksumState(current());

      return new SymbolState(Token::symbol);
    }
  };

  /* -------------------------------------------------------------------------- */
  /*                               Implementation                               */
  /* -------------------------------------------------------------------------- */

  class Nmea0183Lexing : public LexingIface
  {

  public:
    Nmea0183Lexing() = default;
    ~Nmea0183Lexing() = default;

  public:
    /**
     * @brief Detects token type of passed slice
     *
     * @param view Slice of message to detect type of
     * @return Type detected type of token
     */
    Token::Type detectTokenType(const std::string &view) override
    {
      if (view.empty())
        return Token::null;

      State *previous = nullptr, *state = new NullState();

      for (auto &&ch : view)
      {
        previous = state;
        state = state->on_char(ch);
        delete previous;
      }
      Token::Type retval = state->current();
      delete state;
      return retval;
    }
    /**
     * @brief Splits source message onto tokens detecting each type of that
     *
     * @throws wellnmea::parse_error If format of message is invalid
     * @param source
     * @return std::list<Token>
     */
    std::list<Token> splitTokens(const std::string &source) override
    {
      std::list<Token> result;

      // Throw on invalid type
      if (source[0] != '$')
        throw parse_error("Invalid message format");

      std::size_t last_delimeter_position = 0, index = 1; // skip $ sign
      const int mes_length = source.length();

      while (index <= mes_length)
      {
        if (source[index] == ',' || source[index] == '*' || source[index] == 0x00)
        {
          std::size_t posfix = source[last_delimeter_position] == '*' ? 0 : 1;
          std::size_t sym_distance = std::distance(&source[last_delimeter_position + posfix], &source[index]);

          std::string token_view = source.substr(last_delimeter_position + posfix, sym_distance);

          last_delimeter_position = index;

          Token::Type type = detectTokenType(token_view);

          result.push_back(Token(type, token_view));
        }

        index++;
      }

      return result;
    }
  };
} // namespace wellnmea
