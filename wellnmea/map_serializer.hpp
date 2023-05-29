#pragma once

#include <stack>

#include <wellnmea/message.hpp>
#include <wellnmea/values/value_visitor.hpp>
#include <wellnmea/values/degrees.hpp>
#include <wellnmea/values/repeated.hpp>
#include <wellnmea/values/latitude.hpp>
#include <wellnmea/values/longitude.hpp>
#include <wellnmea/values/utc.hpp>

using namespace wellnmea::values;
using wellnmea::Message;
using wellnmea::SerializedProperty;
using wellnmea::SerializedResult;

class MapSerializer : public visitor_base,
                      public visitor<_DegreesValue>,
                      public visitor<_LongitudeValue>,
                      public visitor<_LatitudeValue>,
                      public visitor<_UTCValue>,
                      public visitor<RepeatedValue>
{

  std::stack<SerializedProperty> _packed;

public:
  MapSerializer() {}

public:
  void visit(const _DegreesValue *v)
  {
    SerializedProperty result;
    SerializedProperty p{};

    if (v->as<DegreesValue>() != nullptr)
    {
      p.emplace("value", v->cursor().value());
      p.emplace("measure", _DegreesValue::typeToString(v->measure().value()));
    }

    result.emplace(v->name(), p);
    _packed.push(result);
  }

  void visit(const _LongitudeValue *v)
  {
    SerializedProperty result;
    SerializedProperty p{};
    if (v->as<LongitudeValue>() != nullptr)
    {
      p.emplace("position", v->position().value());
      p.emplace("direction", _LongitudeValue::directionToString(v->direction().value()));
    }
    result.emplace(v->name(), p);
    _packed.push(result);
  }

  void visit(const _LatitudeValue *v)
  {
    SerializedProperty result;
    SerializedProperty p{};
    if (v->as<LatitudeValue>() != nullptr)
    {
      p.emplace("position", v->position().value());
      p.emplace("direction", _LatitudeValue::directionToString(v->direction().value()));
    }
    result.emplace(v->name(), p);
    _packed.push(result);
  }

  void visit(const _UTCValue *v)
  {
    SerializedProperty result;
    SerializedProperty p{};
    if (v->as<UTCValue>() != nullptr)
    {
      p.emplace("hours", v->hours().value_or(0));
      p.emplace("minutes", v->minutes().value_or(0));
      p.emplace("seconds", v->seconds().value_or(0));
      p.emplace("milliseconds", v->milliseconds().value_or(0));
    }
    result.emplace(v->name(), p);
    _packed.push(result);
  }

  void visit(const RepeatedValue *v)
  {
    auto start = v->begin();
    auto end = v->end();
    SerializedProperty result;
    list<vector<SerializedProperty>> values;

    for (auto group = start; group != end; group++)
    {
      vector<SerializedProperty> gresult(group->size());
      NullValue *value = nullptr;
      for (int i = 0; i < group->size(); ++i)
      {
        value = group->at(i);
        value->accept(*this);
        gresult[i] = _packed.top();
        _packed.pop();
      }
      values.push_back(gresult);
    }
    result.emplace(v->name(), values);
    _packed.push(result);
  }

  SerializedResult pack(Message *msg) noexcept
  {

    if (!msg)
    {
      return std::nullopt;
    }

    if (msg->values().size() == 0)
    {
      return std::nullopt;
    }
    SerializedProperty top;

    top.emplace("talker", msg->talker());
    top.emplace("formatter", msg->formatter());

    auto values = msg->values();

    for (auto &&value : values)
    {
      value->accept(*this);
      top.merge(_packed.top());
    }

    return top;
  }
};