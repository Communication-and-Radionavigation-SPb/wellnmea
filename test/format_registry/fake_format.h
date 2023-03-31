#pragma once

#include <wellnmea/formats/format.hpp>


class fake_format: public wellnmea::format {
  public:
  fake_format() = default;
  ~fake_format()  = default;
};