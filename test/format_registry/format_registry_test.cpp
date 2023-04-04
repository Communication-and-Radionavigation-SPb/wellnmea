#include <gtest/gtest.h>

#include <memory>

#include <wellnmea/formats_registry.hpp>
#include <wellnmea/formats/format.hpp>
#include "fake_format.h"

#define Suite FormatsRegistryTest

TEST(Suite, ContainsReturnsFalseForUnregisteredWhenEmpty)
{
  wellnmea::FormatRegistry::clear();
  EXPECT_FALSE(wellnmea::FormatRegistry::contains("dtm"));
}

TEST(Suite, ContainsReturnsFalseForUnregisteredWhenFilled)
{
  wellnmea::FormatRegistry::clear();
  wellnmea::FormatRegistry::connect("dum", std::make_shared<fake_format>());
  EXPECT_FALSE(wellnmea::FormatRegistry::contains("dtm"));
}

TEST(Suite, ContainsReturnsTrueWhenRegisteredFormatRequested)
{
  wellnmea::FormatRegistry::clear();
  wellnmea::FormatRegistry::connect("dum", std::make_shared<fake_format>());
  EXPECT_TRUE(wellnmea::FormatRegistry::contains("dum"));
}

TEST(Suite, ContainsIsCaseInsensitive)
{
  wellnmea::FormatRegistry::clear();
  wellnmea::FormatRegistry::connect("DUM", std::make_shared<fake_format>());
  EXPECT_TRUE(wellnmea::FormatRegistry::contains("dum"));
}

TEST(Suite, ConnectDoNotAllowsDuplicatedFormats)
{
  wellnmea::FormatRegistry::clear();
  wellnmea::FormatRegistry::connect("dum", std::make_shared<fake_format>());

  EXPECT_THROW({
    wellnmea::FormatRegistry::connect("DUM", std::make_shared<fake_format>());
  },
               wellnmea::format_duplication);
}

TEST(Suite, GetFormatThrowsOnUnregisteredRequested)
{
  wellnmea::FormatRegistry::clear();
  EXPECT_THROW({
    wellnmea::FormatRegistry::getFormat("dum");
  },
               wellnmea::format_unregistered);
}