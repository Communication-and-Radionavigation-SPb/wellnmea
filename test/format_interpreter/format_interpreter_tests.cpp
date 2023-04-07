#include <gtest/gtest.h>

#include <vector>
#include <wellnmea/formats/lexem.hpp>
#include <wellnmea/formats/format_interpreter.hpp>

#define Suite FormatInterpreter

using namespace std;
using namespace wellnmea::formats;

TEST(Suite, CorrectlyInterpretsSingleFieldConfiguration)
{
  auto spec = FormatInterpreter::interpret("time|utc");

  ASSERT_EQ(spec.size(), 1);

  EXPECT_EQ(spec[0]->type, Lexem::field);
  auto sub = spec[0]->sublexems;

  ASSERT_TRUE(sub.size() == 2);
  EXPECT_EQ(sub[0]->type, Lexem::word);
  EXPECT_EQ(sub[1]->type, Lexem::word);
}

TEST(Suite, CorrectlyInterpretsMultipleFieldsConfig)
{
  auto spec = FormatInterpreter::interpret("time|utc;longitude|longitude");

  ASSERT_EQ(spec.size(), 2);
  ASSERT_EQ(spec[0]->type, Lexem::field);
  ASSERT_EQ(spec[1]->type, Lexem::field);

  auto lexems1 = spec[0]->sublexems;
  auto lexems2 = spec[1]->sublexems;

  ASSERT_EQ(lexems1.size(), 2);
  ASSERT_EQ(lexems2.size(), 2);

  EXPECT_EQ(lexems1[0]->type, Lexem::word);
  EXPECT_EQ(lexems1[0]->slice, "time");
  EXPECT_EQ(lexems1[1]->type, Lexem::word);
  EXPECT_EQ(lexems1[1]->slice, "utc");

  EXPECT_EQ(lexems2[0]->type, Lexem::word);
  EXPECT_EQ(lexems2[0]->slice, "longitude");

  EXPECT_EQ(lexems2[1]->type, Lexem::word);
  EXPECT_EQ(lexems2[1]->slice, "longitude");
}

TEST(Suite, CorrectlyInterpretsParametriesedFieldConfiguration)
{
  auto spec = FormatInterpreter::interpret("time|utc(20,moskow)");

  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::field);
  auto sub = spec[0]->sublexems;

  ASSERT_EQ(sub.size(), 3);

  EXPECT_EQ(sub[0]->type, Lexem::word);
  EXPECT_EQ(sub[0]->slice, "time");
  EXPECT_EQ(sub[1]->type, Lexem::word);
  EXPECT_EQ(sub[1]->slice, "utc");
  EXPECT_EQ(sub[2]->type, Lexem::params);

  auto params = sub[2]->sublexems;

  ASSERT_EQ(params.size(), 2);
  EXPECT_EQ(params[0]->type, Lexem::number);
  EXPECT_EQ(params[0]->slice, "20");
  EXPECT_EQ(params[1]->type, Lexem::word);
  EXPECT_EQ(params[1]->slice, "moskow");
}

TEST(Suite, CorrectlyInterpretsRepeatedConfiguration)
{
  auto spec = FormatInterpreter::interpret("name[time|utc]");

  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::repetition);

  auto subs = spec[0]->sublexems;

  ASSERT_TRUE(subs.size() == 2);

  EXPECT_EQ(subs[0]->type, Lexem::word);
  EXPECT_EQ(subs[0]->slice, "name");

  EXPECT_EQ(subs[1]->type, Lexem::field);
  auto rep = subs[1]->sublexems;

  ASSERT_EQ(rep.size(), 2);

  auto rep_name = rep[0];
  auto rep_fmt = rep[1];

  EXPECT_EQ(rep_name->type, Lexem::word);
  EXPECT_EQ(rep_name->slice, "time");

  EXPECT_EQ(rep_fmt->type, Lexem::word);
  EXPECT_EQ(rep_fmt->slice, "utc");
}

TEST(Suite, CorrectlyInterpretsNestedRepeatedConfiguration)
{
  auto spec = FormatInterpreter::interpret("name1[name2[time|utc]]");
  
  ASSERT_EQ(spec.size(), 1);

  auto repetition_top = spec[0];

  EXPECT_EQ(repetition_top->type, Lexem::repetition);
  ASSERT_EQ(repetition_top->sublexems.size(), 2);

  auto repetition_top_name = repetition_top->sublexems[0];
  auto repetition_top_content = repetition_top->sublexems[1];

  EXPECT_EQ(repetition_top_name->type, Lexem::word);
  EXPECT_EQ(repetition_top_content->type, Lexem::repetition);

  ASSERT_EQ(repetition_top_content->sublexems.size(), 2);
  
  auto nested_name = repetition_top_content->sublexems[0];

  EXPECT_EQ(nested_name->type, Lexem::word);
  EXPECT_EQ(nested_name->slice, "name2");

  auto field_nested = repetition_top_content->sublexems[1];

  EXPECT_EQ(field_nested->type, Lexem::field);
  ASSERT_EQ(field_nested->sublexems.size(), 2);

  auto field_nested_name = field_nested->sublexems[0];
  auto field_nested_fmt = field_nested->sublexems[1];

  EXPECT_EQ(field_nested_name->type, Lexem::word);
  EXPECT_EQ(field_nested_name->slice, "time");

  EXPECT_EQ(field_nested_fmt->type, Lexem::word);
  EXPECT_EQ(field_nested_fmt->slice, "utc");
}

TEST(Suite, CorrectlyInterpretsRepeatedConfigurationWithSeveralFields)
{
  auto spec = FormatInterpreter::interpret("name[time|utc;long|longitude]");
  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::repetition);
  ASSERT_EQ(spec[0]->sublexems.size(), 3);

  auto repetition_name = spec[0]->sublexems[0];
  EXPECT_EQ(repetition_name->type, Lexem::word);
  EXPECT_EQ(repetition_name->slice, "name");

  auto field_first = spec[0]->sublexems[1];

  EXPECT_EQ(field_first->type, Lexem::field);
  ASSERT_EQ(field_first->sublexems.size(), 2);

  auto field_first_name = field_first->sublexems[0];
  EXPECT_EQ(field_first_name->type, Lexem::word);
  EXPECT_EQ(field_first_name->slice, "time");

  auto field_first_fmt = field_first->sublexems[1];
  EXPECT_EQ(field_first_fmt->type, Lexem::word);
  EXPECT_EQ(field_first_fmt->slice, "utc");

  auto field_second = spec[0]->sublexems[2];
  EXPECT_EQ(field_second->type, Lexem::field);
  ASSERT_EQ(field_second->sublexems.size(), 2);

  auto field_second_name = field_second->sublexems[0];
  EXPECT_EQ(field_second_name->type, Lexem::word);
  EXPECT_EQ(field_second_name->slice, "long");

  auto field_second_fmt = field_second->sublexems[1];
  EXPECT_EQ(field_second_fmt->type, Lexem::word);
  EXPECT_EQ(field_second_fmt->slice, "longitude");

}

TEST(Suite, CorrectlyInterpretsNestedParentheses)
{
  EXPECT_THROW({
    auto spec = FormatInterpreter::interpret("time|utc((20,moskow))");
  }, std::invalid_argument);
}

TEST(Suite, CorrectlyInterpretsRepetitionWithParametrisedField)
{
  auto spec = FormatInterpreter::interpret("name[time|utc(20)]");

  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::repetition);

  auto subs = spec[0]->sublexems;

  ASSERT_TRUE(subs.size() == 2);

  EXPECT_EQ(subs[0]->type, Lexem::word);
  EXPECT_EQ(subs[0]->slice, "name");

  EXPECT_EQ(subs[1]->type, Lexem::field);
  auto field = subs[1]->sublexems;

  ASSERT_EQ(field.size(), 3);

  auto field_name = field[0];
  auto field_fmt = field[1];
  auto field_params = field[2];

  EXPECT_EQ(field_name->type, Lexem::word);
  EXPECT_EQ(field_name->slice, "time");

  EXPECT_EQ(field_fmt->type, Lexem::word);
  EXPECT_EQ(field_fmt->slice, "utc");

  EXPECT_EQ(field_params->type, Lexem::params);
  EXPECT_EQ(field_params->sublexems.size(), 1);

  auto field_param = field_params->sublexems[0];

  EXPECT_EQ(field_param->type, Lexem::number);
  EXPECT_EQ(field_param->slice, "20");
}