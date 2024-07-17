#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/formats/xdr.hpp>
#include <wellnmea/parser.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(XDR_Constructor, default_constructable) {
  EXPECT_NO_THROW({ XDR(); });
}

#define Suite XDR_Tests

class XDR_Tests : public ::testing::TestWithParam<json> {
 public:
  XDR format;
  wellnmea::Parser parser;

 public:
  wa::NullValue* get_named(const std::string& name,
                           std::vector<wa::NullValue*> collection) {
    for (size_t i = 0; i < collection.size(); i++) {
      if (collection.at(i)->name() == name)
        return collection.at(i);
    }
    return nullptr;
  }
};

TEST_P(Suite, no_throws) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  EXPECT_NO_THROW({ format.parse(sentence); });
}

TEST_P(Suite, extracts_correct_values) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  auto values = format.parse(sentence);
  auto rep = (*values.begin())->as<wa::RepeatedValue>();
  auto it = param["fields"].begin();
  auto vpos = rep->begin();
  while (vpos != rep->end()) {
    ASSERT_NE(get_named("type", *vpos), nullptr);
    ASSERT_NE(get_named("value", *vpos), nullptr);
    ASSERT_NE(get_named("units", *vpos), nullptr);
    ASSERT_NE(get_named("transduser", *vpos), nullptr);

    auto type = get_named("type", *vpos)->as<wa::CharacterValue>();
    auto value = get_named("value", *vpos)->as<wa::NumberValue>();
    auto units = get_named("units", *vpos)->as<wa::CharacterValue>();
    auto transducer = get_named("transduser", *vpos)->as<wa::AsIsValue>();
    vpos++;

    ASSERT_TRUE(type->symbol().has_value());
    EXPECT_EQ(type->symbol().value(), ((std::string)(*it)["type"]).at(0));

    ASSERT_TRUE(value->getValue().has_value());
    EXPECT_THAT(value->getValue().value(),
                ::testing::DoubleNear((*it)["value"], 0.01));

    std::string units_str = ((std::string)(*it)["units"]);
    if (!units_str.empty()) {
      ASSERT_TRUE(units->symbol().has_value());
      EXPECT_EQ(units->symbol().value(), units_str.at(0));
    }

    ASSERT_TRUE(transducer->contents().has_value());
    EXPECT_EQ(transducer->contents().value(), (*it)["name"]);

    it++;
  }
}

INSTANTIATE_TEST_SUITE_P(
    Container, Suite, ::testing::ValuesIn(getDataFromFile("xdr_data.json")),
    [](const ::testing::TestParamInfo<XDR_Tests::ParamType>& info) {
      std::stringstream ss;
      std::string name = (std::string)info.param["name"];
      ss << "XDR_" << name;
      return ss.str();
    });