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

TEST(Suite, CorrectlyInterpretsRepeatedConfiguration)
{
  auto spec = FormatInterpreter::interpret("name[time|utc]");

  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::field);

  auto field = spec[0]->sublexems;

  ASSERT_TRUE(field.size() == 2);

  EXPECT_EQ(field[0]->type, Lexem::word);
  EXPECT_EQ(field[0]->slice, "name");
  EXPECT_EQ(field[1]->type, Lexem::repetition);

  auto rep = field[1]->sublexems;

  ASSERT_EQ(rep.size(), 1);

  auto rep_field = rep[0];

  EXPECT_EQ(rep_field->type, Lexem::field);

  auto sub = rep_field->sublexems;

  ASSERT_EQ(sub.size(), 2);

  EXPECT_EQ(sub[0]->type, Lexem::word);
  EXPECT_EQ(sub[1]->type, Lexem::word);
}

TEST(Suite, CorrectlyInterpretsNestedRepeatedConfiguration)
{

  auto spec = FormatInterpreter::interpret("name1[name2[time|utc]]");

  ASSERT_EQ(spec.size(), 1);

  auto field_top = spec[0];

  EXPECT_EQ(field_top->type, Lexem::field);
  ASSERT_EQ(field_top->sublexems.size(), 2);

  auto field_top_name = field_top->sublexems[0];
  auto field_top_content = field_top->sublexems[1];

  EXPECT_EQ(field_top_name->type, Lexem::word);
  EXPECT_EQ(field_top_content->type, Lexem::repetition);

  ASSERT_EQ(field_top_content->sublexems.size(), 1);

  auto field_nested = field_top_content->sublexems[0];

  EXPECT_EQ(field_nested->type, Lexem::field);
  ASSERT_EQ(field_nested->sublexems.size(), 2);

  auto field_nested_name = field_nested->sublexems[0];
  auto field_nested_content = field_nested->sublexems[1];

  EXPECT_EQ(field_nested_name->type, Lexem::word);
  EXPECT_EQ(field_nested_name->slice, "name2");

  EXPECT_EQ(field_nested_content->type, Lexem::repetition);
  ASSERT_EQ(field_nested_content->sublexems.size(), 1);

  auto field_nested_2 = field_nested_content->sublexems[0];

  EXPECT_EQ(field_nested_2->type, Lexem::field);
  ASSERT_EQ(field_nested_2->sublexems.size(), 2);

  auto field_nested_2_name = field_nested_2->sublexems[0];
  auto field_nested_2_type = field_nested_2->sublexems[1];

  EXPECT_EQ(field_nested_2_name->type, Lexem::word);
  EXPECT_EQ(field_nested_2_name->slice, "time");

  EXPECT_EQ(field_nested_2_type->type, Lexem::word);
  EXPECT_EQ(field_nested_2_type->slice, "utc");
}

TEST(Suite, CorrectlyInterpretsRepeatedConfigurationWithSeveralFields)
{
  auto spec = FormatInterpreter::interpret("name[time|utc;long|longitude]");
  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::field);
  ASSERT_EQ(spec[0]->sublexems.size(), 2);

  auto field_top_name = spec[0]->sublexems[0];
  auto field_top_content = spec[0]->sublexems[1];

  EXPECT_EQ(field_top_name->type, Lexem::word);
  EXPECT_EQ(field_top_name->slice, "name");

  EXPECT_EQ(field_top_content->type, Lexem::repetition);
  ASSERT_EQ(field_top_content->sublexems.size(), 2);

  auto nested_field_first = field_top_content->sublexems[0];
  auto nested_field_second = field_top_content->sublexems[1];

  EXPECT_EQ(nested_field_first->type, Lexem::field);
  EXPECT_EQ(nested_field_second->type, Lexem::field);

  ASSERT_EQ(nested_field_first->sublexems.size(), 2);
  ASSERT_EQ(nested_field_second->sublexems.size(), 2);

  auto nested_field_first_name = nested_field_first->sublexems[0];
  auto nested_field_first_fmt = nested_field_first->sublexems[1];

  auto nested_field_second_name = nested_field_second->sublexems[0];
  auto nested_field_second_fmt = nested_field_second->sublexems[1];

  EXPECT_EQ(nested_field_first_name->type, Lexem::word);
  EXPECT_EQ(nested_field_first_name->slice, "time");

  EXPECT_EQ(nested_field_first_fmt->type, Lexem::word);
  EXPECT_EQ(nested_field_first_fmt->slice, "utc");

  EXPECT_EQ(nested_field_second_name->type, Lexem::word);
  EXPECT_EQ(nested_field_second_name->slice, "long");

  EXPECT_EQ(nested_field_second_fmt->type, Lexem::word);
  EXPECT_EQ(nested_field_second_fmt->slice, "longitude");
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

TEST(Suite, CorrectlyInterpretsNestedParentheses)
{
  auto spec = FormatInterpreter::interpret("time|utc((20,moskow))");

  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::field);
  auto sub = spec[0]->sublexems;

  ASSERT_EQ(sub.size(), 3);

  EXPECT_EQ(sub[0]->type, Lexem::word);
  EXPECT_EQ(sub[0]->slice, "time");
  EXPECT_EQ(sub[1]->type, Lexem::word);
  EXPECT_EQ(sub[1]->slice, "utc");
  EXPECT_EQ(sub[2]->type, Lexem::params);
  EXPECT_EQ(sub[2]->slice, "(20,moskow)");

  auto parsub = sub[2]->sublexems;

  ASSERT_EQ(parsub.size(), 1);
  EXPECT_EQ(parsub[0]->type, Lexem::params);
  EXPECT_EQ(parsub[0]->slice, "20,moskow");

  auto params = parsub[0]->sublexems;

  ASSERT_EQ(params.size(), 2);
  EXPECT_EQ(params[0]->type, Lexem::number);
  EXPECT_EQ(params[0]->slice, "20");
  EXPECT_EQ(params[1]->type, Lexem::word);
  EXPECT_EQ(params[1]->slice, "moskow");
}

TEST(Suite, CorrectlyInterpretsRepetitionWithSeveralParametrisedFields)
{
  auto spec = FormatInterpreter::interpret(" status | symbol( A , B ) ; "
                                           "this[ target_number | number(0, 99); label |string(6,7,done) ] ");

  ASSERT_EQ(spec.size(), 2);
  /* -------------------------------------------------------------------------- */
  auto field1 = spec[0];
  auto field2 = spec[1];

  EXPECT_EQ(field1->type, Lexem::field);
  EXPECT_EQ(field2->type, Lexem::field);
  /* -------------------------------------------------------------------------- */
  auto field1_sub = field1->sublexems;
  /* -------------------------------------------------------------------------- */
  ASSERT_EQ(field1_sub.size(), 3);

  auto field1_name = field1_sub[0];
  auto field1_fmt = field1_sub[1];
  auto field1_params = field1_sub[2];

  EXPECT_EQ(field1_name->type, Lexem::word);
  EXPECT_EQ(field1_name->slice, "status");

  EXPECT_EQ(field1_fmt->type, Lexem::word);
  EXPECT_EQ(field1_fmt->slice, "symbol");

  EXPECT_EQ(field1_params->type, Lexem::params);
  ASSERT_EQ(field1_params->sublexems.size(), 2);

  auto field1_params_1 = field1_params->sublexems[0];
  auto field1_params_2 = field1_params->sublexems[1];

  EXPECT_EQ(field1_params_1->type, Lexem::word);
  EXPECT_EQ(field1_params_1->slice, "A");

  EXPECT_EQ(field1_params_2->type, Lexem::word);
  EXPECT_EQ(field1_params_2->slice, "B");

  /* -------------------------------------------------------------------------- */
  auto field2_bus = field2->sublexems;
  ASSERT_EQ(field2_bus.size(), 2);

  auto field2_name = field2_bus[0];
  auto field2_content = field2_bus[1];

  EXPECT_EQ(field2_name->type, Lexem::word);
  EXPECT_EQ(field2_name->slice, "this");

  EXPECT_EQ(field2_content->type, Lexem::repetition);
  ASSERT_EQ(field2_content->sublexems.size(), 2);

  auto field2_repetitions = field2_content->sublexems;

  auto repetition_field1 = field2_repetitions[0];
  EXPECT_EQ(repetition_field1->type, Lexem::field);
  ASSERT_EQ(repetition_field1->sublexems.size(), 3);

  auto rep_field1_name = repetition_field1->sublexems[0];
  auto rep_field1_fmt = repetition_field1->sublexems[1];
  auto rep_field1_params = repetition_field1->sublexems[2];

  EXPECT_EQ(rep_field1_name->type, Lexem::word);
  EXPECT_EQ(rep_field1_name->slice, "target_number");

  EXPECT_EQ(rep_field1_fmt->type, Lexem::word);
  EXPECT_EQ(rep_field1_fmt->slice, "number");

  EXPECT_EQ(rep_field1_params->type, Lexem::params);
  ASSERT_EQ(rep_field1_params->sublexems.size(), 2);

  auto rfp1 = rep_field1_params->sublexems[0];
  auto rfp2 = rep_field1_params->sublexems[1];

  EXPECT_EQ(rfp1->type, Lexem::number);
  EXPECT_EQ(rfp1->slice, "0");
  EXPECT_EQ(rfp2->type, Lexem::number);
  EXPECT_EQ(rfp2->slice, "99");

  /* -------------------------------------------------------------------------- */
  auto repetition_field2 = field2_repetitions[1];
  EXPECT_EQ(repetition_field2->type, Lexem::field);
  ASSERT_EQ(repetition_field2->sublexems.size(), 3);

  auto rf_name = repetition_field2->sublexems[0];
  auto rf_fmt = repetition_field2->sublexems[1];
  auto rf_params = repetition_field2->sublexems[2];

  EXPECT_EQ(rf_name->type, Lexem::word);
  EXPECT_EQ(rf_name->slice, "label");

  EXPECT_EQ(rf_fmt->type, Lexem::word);
  EXPECT_EQ(rf_fmt->slice, "string");

  EXPECT_EQ(rf_params->type, Lexem::params);
  ASSERT_EQ(rf_params->sublexems.size(), 3);

  auto rfp21 = rf_params->sublexems[0];
  auto rfp22 = rf_params->sublexems[1];
  auto rfp23 = rf_params->sublexems[2];


  EXPECT_EQ(rfp21->type, Lexem::number);
  EXPECT_EQ(rfp21->slice, "6");
  EXPECT_EQ(rfp22->type, Lexem::number);
  EXPECT_EQ(rfp22->slice, "7");
  EXPECT_EQ(rfp23->type, Lexem::word);
  EXPECT_EQ(rfp23->slice, "done");
}