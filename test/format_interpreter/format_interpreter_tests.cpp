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
  auto spec = FormatInterpreter::interpret("[time|utc]");

  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::repetition);

  auto field = spec[0]->sublexems;

  ASSERT_TRUE(field.size() == 1);

  auto sub = field[0]->sublexems;

  ASSERT_TRUE(sub.size() == 2);

  EXPECT_EQ(sub[0]->type, Lexem::word);
  EXPECT_EQ(sub[1]->type, Lexem::word);
}

TEST(Suite, CorrectlyInterpretsNestedRepeatedConfiguration) {

  auto spec = FormatInterpreter::interpret("[[time|utc]]");

  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::repetition);
  
  ASSERT_EQ(spec[0]->sublexems.size(), 1);

  auto nested = spec[0]->sublexems[0];

  EXPECT_EQ(nested->type, Lexem::repetition);

  auto field = nested->sublexems;

  ASSERT_TRUE(field.size() == 1);

  auto sub = field[0]->sublexems;

  ASSERT_TRUE(sub.size() == 2);

  EXPECT_EQ(sub[0]->type, Lexem::word);
  EXPECT_EQ(sub[1]->type, Lexem::word);
}

TEST(Suite, CorrectlyInterpretsRepeatedConfigurationWithSeveralFields)
{
  auto spec = FormatInterpreter::interpret("[time|utc;long|longitude]");
  ASSERT_EQ(spec.size(), 1);

  ASSERT_EQ(spec[0]->type, Lexem::repetition);

  auto fields = spec[0]->sublexems;

  ASSERT_EQ(fields.size(), 2);

  auto sub1 = fields[0]->sublexems;
  auto sub2 = fields[1]->sublexems;

  ASSERT_EQ(sub1.size(), 2);

  EXPECT_EQ(sub1[0]->type, Lexem::word);
  EXPECT_EQ(sub1[0]->slice, "time");
  EXPECT_EQ(sub1[1]->type, Lexem::word);
  EXPECT_EQ(sub1[1]->slice, "utc");

  ASSERT_EQ(sub2.size(), 2);

  EXPECT_EQ(sub2[0]->type, Lexem::word);
  EXPECT_EQ(sub2[0]->slice, "long");
  EXPECT_EQ(sub2[1]->type, Lexem::word);
  EXPECT_EQ(sub2[1]->slice, "longitude");
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
  auto spec = FormatInterpreter::interpret(" status | symbol( A , B ) ; [ target_number | number(0, 99); label |string(6,7,done) ] ");

  ASSERT_EQ(spec.size(), 2);
  auto field1 = spec[0];
  auto repetition = spec[1];

  EXPECT_EQ(field1->type, Lexem::field);
  ASSERT_EQ(field1->sublexems.size(), 3);

  auto field1_name = field1->sublexems[0];
  auto field1_format = field1->sublexems[1];
  auto field1_params = field1->sublexems[2];

  EXPECT_EQ(field1_name->type, Lexem::word);
  EXPECT_EQ(field1_name->slice, "status");
  
  EXPECT_EQ(field1_format->type, Lexem::word);
  EXPECT_EQ(field1_format->slice, "symbol");
  
  EXPECT_EQ(field1_params->type, Lexem::params);
  ASSERT_EQ(field1_params->sublexems.size(), 2);
  auto field1_params_1 = field1_params->sublexems[0];
  auto field1_params_2 = field1_params->sublexems[1];

  EXPECT_EQ(field1_params_1->type, Lexem::word);
  EXPECT_EQ(field1_params_1->slice, "A");
  
  EXPECT_EQ(field1_params_2->type, Lexem::word);
  EXPECT_EQ(field1_params_2->slice, "B");

  EXPECT_EQ(repetition->type, Lexem::repetition);

  ASSERT_EQ(repetition->sublexems.size(), 2);

  auto field2 = repetition->sublexems[0];

  EXPECT_EQ(field2->type, Lexem::field) ;

  ASSERT_EQ(field2->sublexems.size(), 3);

  auto field2_name = field2->sublexems[0];
  auto field2_format = field2->sublexems[1];
  auto field2_params = field2->sublexems[2];

  EXPECT_EQ(field2_name->type, Lexem::word);
  EXPECT_EQ(field2_name->slice, "target_number");

  EXPECT_EQ(field2_format->type, Lexem::word);
  EXPECT_EQ(field2_format->slice, "number");

  EXPECT_EQ(field2_params->type, Lexem::params);
  
  ASSERT_EQ(field2_params->sublexems.size(), 2);

  auto field2_params_1 = field2_params->sublexems[0];
  auto field2_params_2 = field2_params->sublexems[1];

  EXPECT_EQ(field2_params_1->type, Lexem::number);
  EXPECT_EQ(field2_params_1->slice, "0");

  EXPECT_EQ(field2_params_2->type, Lexem::number);
  EXPECT_EQ(field2_params_2->slice, "99");

  auto field3 = repetition->sublexems[1];

  EXPECT_EQ(field3->type, Lexem::field);
  ASSERT_EQ(field3->sublexems.size(), 3);

  auto field3_name = field3->sublexems[0];
  auto field3_format = field3->sublexems[1];
  auto field3_params = field3->sublexems[2];

  EXPECT_EQ(field3_name->type, Lexem::word);
  EXPECT_EQ(field3_name->slice, "label");

  EXPECT_EQ(field3_format->type, Lexem::word);
  EXPECT_EQ(field3_format->slice, "string");

  EXPECT_EQ(field3_params->type, Lexem::params);

  ASSERT_EQ(field3_params->sublexems.size(), 3);

  auto field3_params_1 = field3_params->sublexems[0];
  auto field3_params_2 = field3_params->sublexems[1];
  auto field3_params_3  = field3_params->sublexems[2];

  EXPECT_EQ(field3_params_1->type, Lexem::number);
  EXPECT_EQ(field3_params_1->slice, "6");

  EXPECT_EQ(field3_params_2->type, Lexem::number);
  EXPECT_EQ(field3_params_2->slice, "7");

  EXPECT_EQ(field3_params_3->type, Lexem::word);
  EXPECT_EQ(field3_params_3->slice, "done");
}