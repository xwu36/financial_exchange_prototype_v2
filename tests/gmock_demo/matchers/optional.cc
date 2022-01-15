#include <algorithm>
#include <iostream>
#include <optional>
#include <random>
#include <set>
#include <variant>
#include <vector>

#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::_;
using ::testing::AnyOf;
using ::testing::ContainerEq;
using ::testing::Contains;
using ::testing::ContainsRegex;
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::FieldsAre;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::HasSubstr;
using ::testing::IsFalse;
using ::testing::IsNull;
using ::testing::IsSubsetOf;
using ::testing::IsTrue;
using ::testing::Le;
using ::testing::Lt;
using ::testing::Matcher;
using ::testing::MatchesRegex;
using ::testing::Ne;
using ::testing::Not;
using ::testing::NotNull;
using ::testing::Optional;
using ::testing::Pair;
using ::testing::Ref;
using ::testing::StartsWith;
using ::testing::StrCaseEq;
using ::testing::StrCaseNe;
using ::testing::StrEq;
using ::testing::StrNe;
using ::testing::Truly;
using ::testing::TypedEq;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
using ::testing::VariantWith;
using ::testing::WhenSorted;
//-----------------------------------------------------------------------------

TEST(OptionalTest, SimpleTest) {
  std::optional<int> opt(1);
  std::optional<int> no_value;
  std::optional<std::string> opt_str = {"AAA"};

  EXPECT_THAT(opt, Optional(1));
  EXPECT_THAT(opt, 1);
  EXPECT_THAT(no_value, Not(Optional(1)));
  EXPECT_THAT(no_value, std::nullopt);

  EXPECT_THAT(opt.has_value(), IsTrue());
  EXPECT_THAT(no_value.has_value(), IsFalse());

  EXPECT_THAT(opt, Optional(Lt(2)));
  EXPECT_THAT(opt_str, (std::string("AAA")));
}

TEST(VariantTest, SimpleTest) {
  std::variant<int, float> var;

  var = 1;
  EXPECT_THAT(var, 1);
  EXPECT_THAT(var, VariantWith<int>(1));
  EXPECT_THAT(std::get<int>(var), 1);
  EXPECT_THAT(var, Not(VariantWith<float>(1)));

  var = 1.1f;
  EXPECT_THAT(var, VariantWith<float>(1.1f));
  EXPECT_THAT(std::get<float>(var), 1.1f);
  EXPECT_THAT(var, 1.1f);
}
//-----------------------------------------------------------------------------
