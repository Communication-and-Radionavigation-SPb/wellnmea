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

    Token(Type type, const string &slice) : type(type), slice(slice) {}
  };
} // namespace wellnmea
