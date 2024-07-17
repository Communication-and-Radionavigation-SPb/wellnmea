#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/sentence.hpp>
#include <wellnmea/util/number_utils.hpp>
#include <iostream>

#define Suite Parser

TEST(Suite, can_be_instantiated)
{
  EXPECT_NO_THROW({
    wellnmea::Parser parser;
  });
}

TEST(Suite, sanitizes_sentence_text)
{
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "$ GPHDT,\t127.0* 3\t1\r\n");

  EXPECT_EQ(sentence.text, "$GPHDT,127.0*31");
}

TEST(Suite, returns_invalid_sentence_when_no_dollar)
{
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "GPHDT,127.0*31\r\n");
  EXPECT_EQ(sentence.isValid(), false);
}

TEST(Suite, payload_checksum_is_empty_by_default) {
wellnmea::Parser parser;
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, "$GPHDT,127.0");

  EXPECT_EQ(sentence.payloadChecksum, 0);
}

TEST(Suite, assigns_checksum_of_payload)
{
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "$GPHDT,127.0*49");
  std::string chk = "49";
  EXPECT_EQ(sentence.payloadChecksum, (uint8_t)wellnmea::util::toInt(chk, 16));
}

TEST(Suite, assigns_talker) {
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "$GPHDT,127.0*49");
  EXPECT_EQ(sentence.talker, "GP");

  parser.parseInto(sentence, "$HEHDT,127.0*49");
  EXPECT_EQ(sentence.talker, "HE");
}

TEST(Suite, assigns_formatter) {
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "$GPHDT,127.0*49");
  EXPECT_EQ(sentence.formatter, "HDT");

  parser.parseInto(sentence, "$HEROT,127.0*49");
  EXPECT_EQ(sentence.formatter, "ROT");
}

TEST(Suite, adds_empty_field_when_only_name_and_comma) {
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;
  parser.parseInto(sentence, "$GPHDT,*63");

  EXPECT_EQ(sentence.talker, "GP");
  EXPECT_EQ(sentence.formatter, "HDT");

  ASSERT_EQ(sentence.fields.size(), 1);
  EXPECT_EQ(*sentence.fields.begin(), "");
}

TEST(Suite, adds_fields_of_sentence) {
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "$GPHDT,127.0,T,,*31");

  std::cout << "Params: " << std::endl;
  for (auto &&p : sentence.fields)
  {
    std::cout << p << std::endl;
  }
  

  ASSERT_EQ(sentence.fields.size(), 4);
  auto itr = sentence.fields.begin();
  EXPECT_EQ(*itr, "127.0");
  EXPECT_EQ(*(++itr), "T");
  EXPECT_EQ(*(++itr), "");
  EXPECT_EQ(*(itr++), "");
}

TEST(Suite, adds_fields_when_no_trailing_comma) {
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "$GPHDT,127.0,T*31");

  std::cout << "Params: " << std::endl;
  for (auto &&p : sentence.fields)
  {
    std::cout << p << std::endl;
  }

  ASSERT_EQ(sentence.fields.size(), 2);
  auto itr = sentence.fields.begin();
  EXPECT_EQ(*itr, "127.0");
  EXPECT_EQ(*(++itr), "T");
}

TEST(Suite, recognized_checksum) {
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;

  parser.parseInto(sentence, "$GPHDT,127.0,T*31");
  std::string chk = "31";
  EXPECT_EQ(sentence.parsedChecksum, (uint32_t)wellnmea::util::toInt(chk, 16));
}

TEST(Suite, final_sentence_is_vaid) {
  wellnmea::Parser parser;
  wellnmea::Sentence sentence;
  
  parser.parseInto(sentence, "$GPGSV,2,1,08,02,74,042,45,04,18,190,36,07,67,279,42,12,29,323,36*77");

  EXPECT_EQ(sentence.fields.size(), 19);
  EXPECT_TRUE(sentence.integrity());
  EXPECT_TRUE(sentence.isValid());
}