#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <wellnmea/formats/gll.hpp>
#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>

#include "helpers.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST(GLL_constructor, can_be_instantiated) {
  EXPECT_NO_THROW({ GLL(); });
}

#define Suite GLL_Tests

class GLL_Tests : public ::testing::TestWithParam<json> {
 public:
  GLL format;
  wellnmea::Parser parser;
};

TEST_P(Suite, no_throws) {
  json param = GetParam();

  wellnmea::Sentence sentence;
  parser.parseInto(sentence, param["raw"]);

  std::cout << "[SENTENCE] " << sentence.text << std::endl;

  EXPECT_NO_THROW({ format.parse(sentence); });
}

INSTANTIATE_TEST_SUITE_P(
    Container, Suite, ::testing::ValuesIn(getDataFromFile("gll_data.json")));