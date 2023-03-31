#include <gtest/gtest.h>

#include <memory>

#include <wellnmea/formats_registry.hpp>
#include <wellnmea/formats/format.hpp>
#include "fake_format.h"

#define Suite FormatsRegistryTest

TEST(Suite, ContainsReturnsFalseForUnregisteredWhenEmpty)
{
  wellnmea::fmt_registry::clear();
  EXPECT_FALSE(wellnmea::fmt_registry::contains("dtm"));
}

TEST(Suite, ContainsReturnsFalseForUnregisteredWhenFilled)
{
  wellnmea::fmt_registry::clear();
  wellnmea::fmt_registry::connect("dum", std::make_shared<fake_format>());
  EXPECT_FALSE(wellnmea::fmt_registry::contains("dtm"));
}

TEST(Suite, ContainsReturnsTrueWhenRegisteredFormatRequested)
{
  wellnmea::fmt_registry::clear();
  wellnmea::fmt_registry::connect("dum", std::make_shared<fake_format>());
  EXPECT_TRUE(wellnmea::fmt_registry::contains("dum"));
}

TEST(Suite, ContainsIsCaseInsensitive)
{
  wellnmea::fmt_registry::clear();
  wellnmea::fmt_registry::connect("DUM", std::make_shared<fake_format>());
  EXPECT_TRUE(wellnmea::fmt_registry::contains("dum"));
}

TEST(Suite, ConnectDoNotAllowsDuplicatedFormats)
{
  wellnmea::fmt_registry::clear();
  wellnmea::fmt_registry::connect("dum", std::make_shared<fake_format>());

  EXPECT_THROW({
    wellnmea::fmt_registry::connect("DUM", std::make_shared<fake_format>());
  },
               wellnmea::format_duplication);
}

TEST(Suite, GetFormatThrowsOnUnregisteredRequested)
{
  wellnmea::fmt_registry::clear();
  EXPECT_THROW({
    wellnmea::fmt_registry::getFormat("dum");
  },
               wellnmea::format_unregistered);
}