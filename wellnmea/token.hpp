#pragma once

#include <string>
#include <wellnmea/exceptions.hpp>

using std::string;
namespace wellnmea
{
  struct Token
  {
    enum Type
    {
      number,
      symbol,
      checksum,
      null,
      phrase,
    };
    Type type;
    std::string slice;

    Token(Type a_type, const string &a_slice) : type(a_type), slice(a_slice) {}
  };
} // namespace wellnmea
