#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <wellnmea/formats/gga.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(GGA_constructor, can_be_instantiated) {
  EXPECT_NO_THROW({ GGA(); });
}

#define Suite GGA_Tests

/* -------------------------------------------------------------------------- */
/*                               Test not throws                              */
/* -------------------------------------------------------------------------- */

/* ---------------------------------- Test ---------------------------------- */
class GGA_Tests : public ::testing::TestWithParam<json> {
 public:
  GGA format;
  wellnmea::Parser parser;
};

TEST_P(Suite, no_throws) {
  json param = GetParam();

  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;

  EXPECT_NO_THROW({ format.parse(sentence); })
      << sentence.text << " failed to parse";
}

TEST_P(Suite, extracts_correct_utc) {
  json param = GetParam();

  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;
  auto values = format.parse(sentence);
  auto utc = values.begin()->get()->as<wellnmea::instructions::UtcValue>();
  ASSERT_NE(utc, nullptr);
  EXPECT_EQ(utc->getHours(), param["hours"]);
}

TEST_P(Suite, extracts_correct_minutes) {
  json param = GetParam();

  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;
  auto values = format.parse(sentence);
  auto utc = values.begin()->get()->as<wellnmea::instructions::UtcValue>();
  ASSERT_NE(utc, nullptr);
  EXPECT_EQ(utc->getMinutes(), param["minutes"]);
}

TEST_P(Suite, extracts_correct_seconds) {
  json param = GetParam();

  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;
  auto values = format.parse(sentence);
  auto utc = values.begin()->get()->as<wellnmea::instructions::UtcValue>();
  ASSERT_NE(utc, nullptr);
  EXPECT_EQ(utc->getSeconds(), param["seconds"]);
}

INSTANTIATE_TEST_SUITE_P(
    Container, Suite, ::testing::ValuesIn(getDataFromFile("gga_data.json")),
    [](const ::testing::TestParamInfo<GGA_Tests::ParamType>& info) {
      std::stringstream ss;
      ss << "GGA_" << info.index;
      std::string name = ss.str();
      return name;
    });
