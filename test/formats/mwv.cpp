#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/formats/mwv.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define Suite MWV_Tests

TEST(MWV_Constructor, do_not_throws) {
  EXPECT_NO_THROW({ MWV(); });
}

class MWV_Tests : public ::testing::TestWithParam<json> {
 public:
  MWV format;
  wellnmea::Parser parser;
};

TEST_P(Suite, no_throws) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;

  EXPECT_NO_THROW({ format.parse(sentence); });
}

TEST_P(Suite, extracts_correct_angle) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;
  std::cout << "[ANGLE]    " << param["angle"] << std::endl;

  auto values = format.parse(sentence);

  auto it = values.begin();

  EXPECT_THAT(
      (*it)->as<wellnmea::instructions::DegreesValue>()->value.value_or(-1),
      ::testing::DoubleNear(param["angle"], 0.01));
}

TEST_P(Suite, extracts_correct_reference) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << " [SENTENCE] " << sentence.text << std::endl;
  std::cout << "[REFERENCE] " << param["reference"] << std::endl;

  auto values = format.parse(sentence);

  auto it = values.begin();
  ++it;
  std::string reference = param["reference"];
  EXPECT_EQ(
      (*it)->as<wellnmea::instructions::CharacterValue>()->symbol().value_or(
          'x'),
      reference.at(0));
}

TEST_P(Suite, extracts_correct_speed) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;
  std::cout << "   [SPEED] " << param["speed"] << std::endl;

  auto values = format.parse(sentence);

  auto it = values.begin();
  ++it;
  ++it;
  EXPECT_THAT(
      (*it)->as<wellnmea::instructions::SpeedValue>()->raw().value_or(-1),
      ::testing::DoubleNear(param["speed"], 0.01));
}

INSTANTIATE_TEST_SUITE_P(Container, Suite,
                         ::testing::ValuesIn(getDataFromFile("mwv_data.json")));