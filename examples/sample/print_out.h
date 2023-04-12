#pragma once

#include <string>
#include <stack>
#include <iostream>
#include <wellnmea/values/value_visitor.hpp>
#include <wellnmea/values/degrees.hpp>

using namespace std;

class ValuesPrinter : wellnmea::values::visitor_base
{
  public:
    ValuesPrinter() {}
  private:
    stack<string> _packed;

};