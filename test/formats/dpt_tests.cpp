#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <wellnmea/formats/dpt.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(DPT_constructor, can_be_instantiated) {
  EXPECT_NO_THROW({ DPT(); });
}

#define Suite DPT_Tests

class DPT_Tests : public ::testing::TestWithParam<json> {
 public:
  DPT format;
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

  auto meters = get_named_value("depth", values);
  auto offset = get_named_value("offset", values);

  ASSERT_NE(meters, nullptr);
  EXPECT_THAT(
      meters->as<wellnmea::instructions::NumberValue>()->getValue().value(),
      ::testing::DoubleNear(param["depth"], 0.01));
  ASSERT_NE(offset, nullptr);
  EXPECT_THAT(
      offset->as<wellnmea::instructions::NumberValue>()->getValue().value(),
      ::testing::DoubleNear(param["offset"], 0.01));
}

INSTANTIATE_TEST_SUITE_P(
    Container, Suite, ::testing::ValuesIn(getDataFromFile("dpt_data.json")),
    [](const ::testing::TestParamInfo<DPT_Tests::ParamType>& info) {
      std::stringstream ss;
      ss << "DPT_" << info.index;
      std::string name = ss.str();
      return name;
    });
