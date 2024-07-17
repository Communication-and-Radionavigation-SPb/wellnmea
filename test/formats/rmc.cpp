#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/formats/rmc.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define Suite RMC_Tests

TEST(RMC_Constructor, do_not_throws) {
  EXPECT_NO_THROW({ RMC(); });
}

class RMC_Tests : public ::testing::TestWithParam<json> {
 public:
  RMC format;
  wellnmea::Parser parser;
};

TEST_P(Suite, no_throws) {
  json param = GetParam();
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;

  EXPECT_NO_THROW({ format.parse(sentence); });
}

TEST_P(Suite, extracts_correct_values) {
  json param = GetParam();
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;

  auto values = format.parse(sentence);

  auto it = values.begin();

  auto utc = (*it)->as<wellnmea::instructions::UtcValue>();
  auto status = (*++it)->as<wellnmea::instructions::CharacterValue>();
  auto latitude = (*++it)->as<wellnmea::instructions::LatitudeValue>();
  auto longitude = (*++it)->as<wellnmea::instructions::LongitudeValue>();
  auto speed = (*++it)->as<wellnmea::instructions::SpeedValue>();
  auto track = (*++it)->as<wellnmea::instructions::DegreesValue>();
  auto date = (*++it)->as<wellnmea::instructions::DateValue>();

  EXPECT_THAT(utc->getTimestamp().value(),
              ::testing::DoubleNear(param["utc"], 0.0001));
  EXPECT_THAT(
      latitude->direction().value(),
      ::testing::Eq(param["latitude_direction"].get<std::string>().at(0)));
  EXPECT_THAT(
      longitude->direction().value(),
      ::testing::Eq(param["longitude_direction"].get<std::string>().at(0)));
  EXPECT_THAT(
      speed->converted().value(),
      ::testing::DoubleNear(param["speed"].get<double>() * 1.852, 0.0001));
  EXPECT_THAT(date->day(), param["day"].get<int>());
  EXPECT_THAT(date->month(), param["month"].get<int>());
  EXPECT_THAT(date->year(), param["year"].get<int>());
}

INSTANTIATE_TEST_SUITE_P(Container, Suite,
                         ::testing::ValuesIn(getDataFromFile("rmc_data.json")));