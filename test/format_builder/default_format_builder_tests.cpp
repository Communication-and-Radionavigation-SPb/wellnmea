#include <gtest/gtest.h>

#include <string>
#include <map>
#include <vector>
#include <wellnmea/exceptions.hpp>
#include <wellnmea/formats/lexem.hpp>
#include <wellnmea/formats/format_interpreter.hpp>
#include <wellnmea/default_format_builder.hpp>

#include <wellnmea/formats/utc_instruction.hpp>
#include <wellnmea/formats/latitude_instruction.hpp>
#include <wellnmea/formats/longitude_instruction.hpp>

using namespace std;
using namespace wellnmea::formats;

#define Suite TestDefaultFormatBuilder

TEST(Suite, ThrowsOnFieldInvalidConfiguration)
{
  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("time|utc|time");

  EXPECT_THROW({
    try
    {
      builder.build(lexems);
    }
    catch (const std::exception &e)
    {
      EXPECT_EQ(std::string(e.what()),
                std::string("Invalid configuration provided."
                            " Invalid field params statement received"
                            " in `time|utc|time`."));
      throw;
    }
  },
               wellnmea::configuration_error);
}

TEST(Suite, ThrowsOnTopLevelLexemInvalidType)
{
  wellnmea::DefaultFormatBuilder builder;
  std::vector<Lexem *> lexems{new Lexem(Lexem::word, "")};

  EXPECT_THROW({
    try
    {
      builder.build(lexems);
    }
    catch (const std::exception &e)
    {
      EXPECT_STREQ(e.what(), "Invalid configuration provided. Error occured at ``.");
      throw;
    }
  },
               wellnmea::configuration_error);
}

TEST(Suite, ThrowsOnInvalidFieldLexemSubCountLess)
{
  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("time");
  EXPECT_THROW({
    try
    {
      builder.build(lexems);
    }
    catch (const std::exception &e)
    {
      EXPECT_STREQ(e.what(), "Invalid configuration provided."
                             " Minimal NMEA field rule is: `name|format`");
      throw;
    }
  },
               wellnmea::configuration_error);
}

TEST(Suite, ThrowsOnInvalidFieldLexemSubCountFew)
{
  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("time|utc(20)()");

  try
  {
    builder.build(lexems);
    FAIL();
  }
  catch (const std::exception &e)
  {
    EXPECT_EQ(std::string(e.what()),
              std::string("Invalid configuration provided. Too few statements for a field in `time|utc(20)()`"));
  }

  EXPECT_THROW({ auto fmt = builder.build(lexems); }, wellnmea::configuration_error);
}

TEST(Suite, ThrowsOnFieldFormatAbsent)
{
  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("time(20)");
  EXPECT_THROW({
    try
    {
      builder.build(lexems);
    }
    catch (const std::exception &e)
    {
      EXPECT_STREQ(e.what(), "Invalid configuration provided."
                             " Unknown field format provided in `time(20)`.");
      throw;
    }
  },
               wellnmea::configuration_error);
}

TEST(Suite, ThrowsOnEmptyRepetition)
{
  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("name[]");

  EXPECT_THROW({
    try
    {
      builder.build(lexems);
    }
    catch (const std::exception &e)
    {
      EXPECT_STREQ(e.what(), "Invalid configuration provided."
                             " Repeated part can not be empty.");
      throw;
    }
  },
               wellnmea::configuration_error);
}

TEST(Suite, RepeatedPartCanContainOnlyFieldLexems)
{
  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("one[two[field|utc]]");

  EXPECT_THROW({
    try
    {
      builder.build(lexems);
    }
    catch (const std::exception &e)
    {
      EXPECT_STREQ(e.what(), "Invalid configuration provided. "
                             "Repeated part can contain only field instructions but "
                             "`two[field|utc]` provided.");
      throw;
    }
  },
               wellnmea::configuration_error);
}

TEST(Suite, CanProduceUtcInstruction)
{
  auto utc = std::make_shared<UTCInstruction>("");

  InstructionsRegistry::add(utc);

  ASSERT_TRUE(InstructionsRegistry::contains("utc"));
  ASSERT_EQ(utc->which(), "utc");

  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("time|utc;other|utc");

  auto fmt = builder.build(lexems);

  EXPECT_EQ(fmt->count(), 2);
}

TEST(Suite, CanProduceLatitudeInstruction)
{

  auto latitude = std::make_shared<LatitudeInstruction>("");

  InstructionsRegistry::add(latitude);

  ASSERT_TRUE(InstructionsRegistry::contains("latitude"));
  ASSERT_EQ(latitude->which(), "latitude");

  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("field_name|latitude");

  auto fmt = builder.build(lexems);

  EXPECT_EQ(fmt->count(), 1);
}

TEST(Suite, CanProduceLongitudeInstruction)
{

  auto instr = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(instr);

  ASSERT_TRUE(InstructionsRegistry::contains("longitude"));
  ASSERT_EQ(instr->which(), "longitude");

  wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("field_name|longitude");

  auto fmt = builder.build(lexems);

  EXPECT_EQ(fmt->count(), 1);
}

TEST(Suite, CanProduceRepetitionInstruction) {
  auto instr = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(instr);


  ASSERT_TRUE(InstructionsRegistry::contains("longitude"));
  ASSERT_EQ(instr->which(), "longitude");

   wellnmea::DefaultFormatBuilder builder;

  auto lexems = FormatInterpreter::interpret("longs[field_name|longitude]");

  auto fmt = builder.build(lexems);

  EXPECT_EQ(fmt->count(), 1);
}