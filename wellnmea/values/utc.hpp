#pragma once

#include <wellnmea/values/null_value.hpp>

namespace wellnmea
{
  namespace values
  {
    class UTCValue : public NullValue
    {
    public:
      int hours;
      int minutes;
      int seconds;

    public:
      UTCValue(const std::string &name) : hours(0), minutes(0), seconds(0),
                                    NullValue(name) {}
    };
  } // namespace params

} // namespace wellnmea
