#pragma once

#include <cstdlib>
#include <sstream>
#include <stdexcept>

namespace wellnmea {
/**
   * @brief Error which represents number from string decode
   * error.
   *
   * Commonly happens when provided string is not valid number
   *
   */
class NumberDecodeError : public std::exception {
 public:
  std::string message;
  NumberDecodeError(const std::string& a_message)
      : message("NumberDecodeError: " + a_message){};

  virtual ~NumberDecodeError() = default;

  const char* what() const noexcept { return message.c_str(); }
};

class NumberRangeError : public std::exception {
 public:
  std::string message;
  NumberRangeError(const std::string& a_message)
      : message("NumberRangeError" + a_message){};

  virtual ~NumberRangeError() = default;

  const char* what() const noexcept { return message.c_str(); }
};

namespace util {

inline double toDouble(const std::string& s) {
  char* p;
  double d = std::strtod(s.c_str(), &p);
  if (*p != 0) {
    std::stringstream ss;
    ss << "toDouble() could not decode provided string `" << s << "`, " << *p
       << " is not a number";
    throw NumberDecodeError(ss.str());
  }

  return d;
}

inline double toDouble(const std::string_view& s) {
  std::string str = std::string(s.begin(), s.end());
  return toDouble(str);
}

inline int64_t toInt(const std::string& s, int radix = 10) {
  char* p;
  int64_t d = std::strtoll(s.c_str(), &p, radix);

  if (*p != 0) {
    std::stringstream ss;
    ss << "toInt() could not decode provided string `" << s << "`, " << *p
       << " is not a number.";
    throw NumberDecodeError(ss.str());
  }

  return d;
}
}  // namespace util
}  // namespace wellnmea
