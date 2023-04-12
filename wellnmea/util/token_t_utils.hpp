#pragma once

#include <wellnmea/token.hpp>

namespace wellnmea
{
  namespace util
  {
    namespace token
    {
      using type = Token::Type;

      inline bool isNull(type t) noexcept
      {
        return t == Token::null;
      }

      inline bool isNum(type t) noexcept
      {
        return t == Token::number;
      }

      inline bool isNullNum(type t) noexcept
      {
        return isNull(t) || isNum(t);
      }

      inline bool isSym(type t) noexcept
      {
        return t == Token::symbol;
      }

      inline bool isNullSym(type t) noexcept
      {
        return isSym(t) || isNull(t);
      }

      inline bool isChecksum(type t) noexcept
      {
        return t == Token::checksum;
      }
    } // namespace token

  } // namespace utils

} // namespace wellnmea
