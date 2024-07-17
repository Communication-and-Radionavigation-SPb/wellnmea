#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include <wellnmea/formats/hrm.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(HRM_Constructor, can_be_default_instantiated) {
  EXPECT_NO_THROW({ HRM(); });
}

#define Suite HRM_Tests

class HRM_Tests : public ::testing::TestWithParam<json> {
 public:
  HRM format;
  wellnmea::Parser parser;

  std::list<wellnmea::formats::Format::ValuePtr> parse(json param) {
    wellnmea::Sentence sentence;
    parser.parseInto(sentence, param["raw"]);

    return format.parse(sentence);
  }

  wellnmea::FmtValue get_named_value(const std::string& name,
                                     wellnmea::FmtValues& values) {
    for (auto&& v : values) {
      if (v->name() == name) {
        return v;
      }
    }
    return nullptr;
  }
};

TEST_P(Suite, no_throws) {
  json param = GetParam();

  wellnmea::Sentence sentence;

  parser.parseInto(sentence, param["raw"]);

  std::cout << " [SENTENCE] " << sentence.text << std::endl;

  EXPECT_NO_THROW({ format.parse(sentence); })
      << sentence.text << " failed to parse";
}

TEST_P(Suite, extracts_correct_values) {
  auto param = GetParam();
  auto values = parse(param);

  auto roll = get_named_value("roll", values)
                  ->as<wellnmea::instructions::NumberValue>();
  auto period = get_named_value("period", values)
                    ->as<wellnmea::instructions::NumberValue>();
  auto amplitude = get_named_value("amplitude", values)
                       ->as<wellnmea::instructions::NumberValue>();
  auto heel = get_named_value("heel", values)
                  ->as<wellnmea::instructions::NumberValue>();

  ASSERT_NE(roll, nullptr) << "roll value not found";
  ASSERT_NE(period, nullptr) << "period value not found";
  ASSERT_NE(amplitude, nullptr) << "amplitude value not found";
  ASSERT_NE(heel, nullptr) << "heel value not found";

  EXPECT_FALSE(roll->empty());
  EXPECT_FALSE(period->empty());
  EXPECT_FALSE(amplitude->empty());
  EXPECT_FALSE(heel->empty());

  EXPECT_THAT(roll->getValue().value(),
              ::testing::DoubleNear(param["roll"]["value"], 0.01));
  EXPECT_THAT(period->getValue().value(),
              ::testing::DoubleNear(param["roll"]["period"], 0.01));
  EXPECT_THAT(amplitude->getValue().value(),
              ::testing::DoubleNear(param["roll"]["amplitude"], 0.01));
  EXPECT_THAT(heel->getValue().value(),
              ::testing::DoubleNear(param["roll"]["heel"], 0.01));
}

INSTANTIATE_TEST_SUITE_P(
    Container, Suite, ::testing::ValuesIn(getDataFromFile("hrm_data.json")),
    [](const ::testing::TestParamInfo<HRM_Tests::ParamType>& info) {
      std::stringstream ss;
      ss << "HRM_" << info.index;
      std::string name = ss.str();
      return name;
    });