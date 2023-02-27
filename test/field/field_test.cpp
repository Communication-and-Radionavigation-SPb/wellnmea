#include <gtest/gtest.h>

#include <wellnmea/field.hpp>

TEST(FieldTests, InstantiatesCorrectly) {
  EXPECT_NO_THROW({
    wellnmea::field("temperature", 1);
  });
}

TEST(FieldTest, ThrowsOnInvalidPosition) {
  EXPECT_ANY_THROW({
    wellnmea::field("temp", 0);
  });
  EXPECT_ANY_THROW({
    wellnmea::field("temp", -1);
  });
}

TEST(FieldTest, DoNotThrowOnValidPosition) {
  EXPECT_NO_THROW({
    wellnmea::field("temp", 1);
  });
}

TEST(FieldTest, ThrowsOnEmptyFieldName) {
  EXPECT_ANY_THROW({
    wellnmea::field("", 1);
  });
}

TEST(FieldTest, UnrequiredByDefault) {
  wellnmea::field f("temp", 1);

  EXPECT_FALSE(f.required()) << "Field must be unrequired by default";
}

TEST(FieldTest, CanBeMarkedAsRequired) {
  wellnmea::field f("temp", 1);
  f.markRequired();
  EXPECT_TRUE(f.required()) << "Field must become required when marked as such";
}

TEST(FieldTest, ReturnsProperFieldName) {
  wellnmea::field f("temp", 1);
  EXPECT_EQ(f.fieldName(), "temp");
}

TEST(FieldTest, ReturnsProperPosition) {
  wellnmea::field f("temp", 1);
  EXPECT_EQ(f.position(), 1);
}