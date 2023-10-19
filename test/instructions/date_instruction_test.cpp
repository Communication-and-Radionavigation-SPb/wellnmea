#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "helpers.hpp"

#include <nlohmann/json.hpp>
#include <wellnmea/instructions/date_instruction.hpp>
#include <wellnmea/sentence.hpp>

using json = nlohmann::json;

#define Base BaseDateInstruction_Tests

TEST(Base, can_be_instantiated) {
  EXPECT_NO_THROW({ wellnmea::instructions::DateInstruction instr(""); });
}

TEST(Base, can_be_cloned) {
  test_clonable<wellnmea::instructions::DateInstruction>();
}

TEST(Base, which_expected_to_be_correct) {
  wellnmea::instructions::DateInstruction instr("");

  EXPECT_EQ(instr.which(), "date");
}

#define Suite DateInstruction_Tests

class DateInstruction_Tests : public ::testing::TestWithParam<json> {
 public:
  wellnmea::instructions::DateInstruction* instruction;
  wellnmea::Sentence* sentence;
  std::list<std::string_view>::iterator it;
  std::list<std::string_view>::const_iterator end;
  std::string field;

 protected:
  void SetUp() override {
    json param = GetParam();
    instruction = new wellnmea::instructions::DateInstruction("test");
    sentence = new wellnmea::Sentence;
    field = param["field"].get<std::string>();
    std::cout << "[FIELD] " << field << std::endl;
    sentence->fields.push_back(field);

    it = sentence->fields.begin();
    end = sentence->fields.end();
  }

  void TearDown() override {
    delete instruction;
    delete sentence;
  }
};

TEST_P(Suite, moves_iterator_forward) {
  instruction->extract(it, end);

  EXPECT_EQ(it, end);
}

TEST_P(Suite, returns_non_null_address) {
  auto value = instruction->extract(it, end);

  ASSERT_NE(value, nullptr);
}

TEST_P(Suite, returns_valid_value) {
  auto value = instruction->extract(it, end);

  EXPECT_NE(value->as<wellnmea::instructions::DateValue>(), nullptr);
}

TEST_P(Suite, returns_value_with_valid_attributes) {
  auto param = GetParam();

  auto value =
      instruction->extract(it, end)->as<wellnmea::instructions::DateValue>();

  EXPECT_EQ(value->name(), instruction->name());

  EXPECT_TRUE(value->has_value());

  EXPECT_EQ(value->day(), param["day"]);
  EXPECT_EQ(value->month(), param["month"]);
  EXPECT_EQ(value->year(), param["year"]);
}

std::vector<json> getDataFromFile(const std::string& filename) {
  std::vector<json> data;
  std::ifstream input(filename);
  json j;
  input >> j;
  if (!j.is_array())
    return {};

  for (json& item : j) {
    data.push_back(item);
  }

  return data;
}

INSTANTIATE_TEST_SUITE_P(
    Container, Suite,
    ::testing::ValuesIn(getDataFromFile("date_instruction_data.json")),
    [](const ::testing::TestParamInfo<DateInstruction_Tests::ParamType>& info) {
      std::stringstream ss;
      std::string dt = info.param["field"];
      ss << dt << "_" << info.index;
      return ss.str();
    });