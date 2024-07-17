#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/formats/rot.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(ROT_constructor, can_be_instantiated) {
  EXPECT_NO_THROW({ ROT(); });
}

#define Suite ROT_Tests

class ROT_Tests : public ::testing::TestWithParam<json> {
 public:
  ROT format;
  wellnmea::Parser parser;

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

  std::cout << "[SENTENCE] " << sentence.text << std::endl;

  EXPECT_NO_THROW({ format.parse(sentence); });
}

TEST_P(Suite, parsed_correctly) {
  json param = GetParam();
  auto degrees_value = param.at("value").get<double>();

  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;

  EXPECT_NO_THROW({
    auto parsed = format.parse(sentence);
    auto degrees = get_named_value("degrees", parsed);
    EXPECT_EQ(
        degrees->as<wellnmea::instructions::DegreesValue>()->value.value(),
        degrees_value);
  });
}

INSTANTIATE_TEST_SUITE_P(Container, Suite,
                         ::testing::ValuesIn(getDataFromFile("rot_data.json")));