#pragma once

#include <string>
#include <wellnmea/token.hpp>

namespace wellnmea
{
  class LexingIface {
    public:
      virtual Token::Type detectTokenType(const std::string &view) = 0;
      virtual std::list<Token> splitTokens(const std::string &source) = 0;
  };

} // namespace wellnmea
