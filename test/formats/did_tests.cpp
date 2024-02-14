#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <wellnmea/formats/did.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(DID_constructor, can_be_instantiated) {
  EXPECT_NO_THROW({ DID(); });
}

class DID_Tests : public ::testing::TestWithParam<json> {
 public:
  DID format;
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

TEST_P(DID_Tests, no_throws) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  EXPECT_NO_THROW({ format.parse(sentence); })
      << sentence.text << " failed to parse";
}

TEST_P(DID_Tests, extracts_correct_values) {
  auto param = GetParam();
  auto values = parse(param);

  auto pitch = get_named_value("pitch", values);
  auto roll = get_named_value("roll", values);
  auto heading = get_named_value("heading", values);

  ASSERT_NE(pitch, nullptr);
  EXPECT_THAT(pitch->as<wellnmea::instructions::DegreesValue>(),
              ::testing::Ne(nullptr));
  EXPECT_THAT(pitch->as<wellnmea::instructions::DegreesValue>()->value.value(),
              ::testing::DoubleNear(param["pitch"], 0.001));

  ASSERT_NE(roll, nullptr);
  EXPECT_THAT(roll->as<wellnmea::instructions::DegreesValue>(),
              ::testing::Ne(nullptr));
  EXPECT_THAT(roll->as<wellnmea::instructions::DegreesValue>()->value.value(),
              ::testing::DoubleNear(param["roll"], 0.001));

  ASSERT_NE(heading, nullptr);
  EXPECT_THAT(heading->as<wellnmea::instructions::DegreesValue>(),
              ::testing::Ne(nullptr));
  EXPECT_THAT(
      heading->as<wellnmea::instructions::DegreesValue>()->value.value(),
      ::testing::DoubleNear(param["heading"], 0.001));
}

INSTANTIATE_TEST_SUITE_P(
    Container, DID_Tests, ::testing::ValuesIn(getDataFromFile("did_data.json")),
    [](const ::testing::TestParamInfo<DID_Tests::ParamType>& info) {
      std::stringstream ss;
      ss << "DID_" << info.index;
      std::string name = ss.str();
      return name;
    });