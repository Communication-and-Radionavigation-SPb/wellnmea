#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <wellnmea/formats/dbs.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(DBS_constructor, can_be_instantiated) {
  EXPECT_NO_THROW({ DBS(); });
}

#define Suite DBS_Tests

class DBS_Tests : public ::testing::TestWithParam<json> {
 public:
  DBS format;
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

  auto feets = get_named_value("feet", values);
  auto meters = get_named_value("meters", values)
                    ->as<wellnmea::instructions::DistanceValue>();
  auto fathoms = get_named_value("fathoms", values)
                     ->as<wellnmea::instructions::DistanceValue>();

  ASSERT_NE(feets, nullptr);
  EXPECT_THAT(
      feets->as<wellnmea::instructions::DistanceValue>()->units().value(),
      ::testing::Eq('f'));
  EXPECT_THAT(feets->as<wellnmea::instructions::DistanceValue>()->raw().value(),
              testing::DoubleNear(param["feets"], 0.01));

  ASSERT_NE(meters, nullptr);
  EXPECT_THAT(
      meters->as<wellnmea::instructions::DistanceValue>()->units().value(),
      ::testing::Eq('M'));
  EXPECT_THAT(
      meters->as<wellnmea::instructions::DistanceValue>()->raw().value(),
      testing::DoubleNear(param["meters"], 0.01));

  ASSERT_NE(fathoms, nullptr);
  EXPECT_THAT(
      fathoms->as<wellnmea::instructions::DistanceValue>()->units().value(),
      ::testing::Eq('F'));
  EXPECT_THAT(
      fathoms->as<wellnmea::instructions::DistanceValue>()->raw().value(),
      testing::DoubleNear(param["fathoms"], 0.01));
}

INSTANTIATE_TEST_SUITE_P(
    Container, Suite, ::testing::ValuesIn(getDataFromFile("dbs_data.json")),
    [](const ::testing::TestParamInfo<DBS_Tests::ParamType>& info) {
      std::stringstream ss;
      ss << "DBS_" << info.index;
      std::string name = ss.str();
      return name;
    });
