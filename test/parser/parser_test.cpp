#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <wellnmea/parser.hpp>
#include <wellnmea/message.hpp>
#include <wellnmea/nmea0183_lexing.hpp>
#include <wellnmea/formats/degreese_instruction.hpp>
#include <wellnmea/formats/longitude_instruction.hpp>
#include <wellnmea/formats/instructions_registry.hpp>
#include <wellnmea/formats/format_interpreter.hpp>
#include <wellnmea/default_format_builder.hpp>
#include <wellnmea/formats/formats_registry.hpp>
#include <wellnmea/map_serializer.hpp>

#define Suite Parser

using wellnmea::Message;
using namespace wellnmea::formats;
using namespace wellnmea::values;

TEST(Suite, InstantiatesWithoutExceptions)
{
  EXPECT_NO_THROW({
    auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();
    wellnmea::Parser parser(lex, {});
  });
}

TEST(Suite, ThrowsOnInvalidMessageFormat)
{

  auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();

  auto dgrs = std::make_shared<DegreesInstruction>("");
  auto lng = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(dgrs);
  InstructionsRegistry::add(lng);

  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("block[direction|degrees;position|longitude;];");

  auto fmt = builder.build(lexems);

  FormatRegistry::connect("DTM", fmt);

  wellnmea::Parser parser(lex, {});

  EXPECT_THROW({ parser.parse("$TEDTM,W84,,,,,,,*17"); }, wellnmea::extraction_error);
}

TEST(Suite, ReturnsCorrectParsedMessage)
{

  FormatRegistry::clear();

  auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();

  auto dgrs = std::make_shared<DegreesInstruction>("");
  auto lng = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(dgrs);
  InstructionsRegistry::add(lng);

  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("block[direction|degrees;position|longitude;];");

  auto fmt = builder.build(lexems);

  FormatRegistry::connect("DTM", fmt);

  wellnmea::Parser parser(lex, {});

  std::shared_ptr<Message> msg = parser.parse("$TEDTM,84,M,98.9,E,54.9,T,55.9,W*17");

  EXPECT_EQ(msg->talker(), "TE");
  EXPECT_EQ(msg->formatter(), "DTM");
  EXPECT_EQ(msg->values().size(), 1);
  auto values = msg->values();
  auto pos = values.begin()->get()->as<RepeatedValue>();
  auto it = pos->begin();
}

TEST(Suite, MapSerializerReturnsRightValueForDegrees)
{
  FormatRegistry::clear();

  auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();

  auto dgrs = std::make_shared<DegreesInstruction>("");
  auto lng = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(dgrs);
  InstructionsRegistry::add(lng);

  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("direction|degrees");

  auto fmt = builder.build(lexems);

  FormatRegistry::connect("DTM", fmt);

  wellnmea::Parser parser(lex, {});

  std::shared_ptr<Message> msg = parser.parse("$TEDTM,84,M");
  EXPECT_EQ(msg->talker(), "TE");
  EXPECT_EQ(msg->formatter(), "DTM");
  EXPECT_EQ(msg->values().size(), 1);

  MapSerializer s;
  using degrees_visitor = visitor<_DegreesValue>;

  ASSERT_NE(dynamic_cast<degrees_visitor *>(&s), nullptr);

  auto result = s.pack(msg.get());
  EXPECT_NE(result, std::nullopt);
  auto map = result.value();

  EXPECT_NE(map.find("talker"), map.end());
  EXPECT_NE(map.find("formatter"), map.end());
  EXPECT_NE(map.find("direction"), map.end());

  auto direction = std::any_cast<std::map<std::string, std::any>>(map["direction"]);

  EXPECT_NE(direction.find("value"), direction.end());
  EXPECT_NE(direction.find("measure"), direction.end());

  auto value = std::any_cast<double>(direction["value"]);
  auto measure = std::any_cast<string>(direction["measure"]);

  EXPECT_EQ(value, 84);
  EXPECT_EQ(measure, "Magnetic");
}

TEST(Suite, default_map_message_serializer_returns_right_value)
{
  FormatRegistry::clear();
  auto lex = std::make_shared<wellnmea::Nmea0183Lexing>();

  auto dgrs = std::make_shared<DegreesInstruction>("");
  auto lng = std::make_shared<LongitudeInstruction>("");

  InstructionsRegistry::add(dgrs);
  InstructionsRegistry::add(lng);

  wellnmea::DefaultFormatBuilder builder;
  auto lexems = FormatInterpreter::interpret("block[direction|degrees;];");

  auto fmt = builder.build(lexems);

  FormatRegistry::connect("DTM", fmt);

  wellnmea::Parser parser(lex, {});

  std::shared_ptr<Message> msg = parser.parse("$TEDTM,84,M,54.9,T*17");

  MapSerializer s;

  auto result = s.pack(msg.get());

  EXPECT_NE(result, std::nullopt);
  auto map = result.value();

  EXPECT_NE(map.find("talker"), map.end());
  EXPECT_NE(map.find("formatter"), map.end());
  EXPECT_NE(map.find("block"), map.end());

  auto block = std::any_cast<list<vector<SerializedProperty>>>(map["block"]);
  ASSERT_EQ(block.size(), 2);
  vector<double> cursors{84, 54.9};
  vector<string> measurements{"Magnetic", "True"};
  auto cit = cursors.begin();
  auto mit = measurements.begin();

  for (auto it = block.begin(); it != block.end(); it++)
  {
    auto group = (*it)[0];
    
    ASSERT_NE(group.find("direction"), group.end());

    auto direction = std::any_cast<SerializedProperty>(group["direction"]);

    ASSERT_NE(direction.find("value"), direction.end());
    ASSERT_NE(direction.find("measure"), direction.end());

    auto value = direction["value"];
    auto measure = direction["measure"];

    EXPECT_EQ(std::any_cast<double>(value), *cit);
    EXPECT_EQ(std::any_cast<string>(measure), *mit);
    cit++;
    mit++;
  }
  
}
