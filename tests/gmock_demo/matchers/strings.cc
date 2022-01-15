#include <algorithm>
#include <iostream>
#include <random>
#include <set>
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
using ::testing::Gt;
using ::testing::HasSubstr;
using ::testing::IsSubsetOf;
using ::testing::Lt;
using ::testing::MatchesRegex;
using ::testing::Ne;
using ::testing::Not;
using ::testing::Pair;
using ::testing::StartsWith;
using ::testing::StrCaseEq;
using ::testing::StrCaseNe;
using ::testing::StrEq;
using ::testing::StrNe;
using ::testing::Truly;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
using ::testing::WhenSorted;
//-----------------------------------------------------------------------------
TEST(MatchersTest, StringMatchers) {
  auto str1 = std::string("This is aa test. Thanks for watching.");
  auto str2 = std::string("This is bb test.");

  EXPECT_THAT(str1, ContainsRegex(".*aa test\\."));
  EXPECT_THAT(str1, ContainsRegex("[a]+ test\\."));
  EXPECT_THAT(str1, ContainsRegex("a{2} test\\."));
  EXPECT_THAT(str1, ContainsRegex("a{1,} test\\."));

  EXPECT_THAT(str1, ContainsRegex("(aa|bb) test\\."));
  EXPECT_THAT(str2, ContainsRegex("(aa|bb) test\\."));

  EXPECT_THAT(str1, MatchesRegex("T.*\\."));

  EXPECT_THAT(str1, EndsWith("watching."));
  EXPECT_THAT(str1, StartsWith("This is"));

  EXPECT_THAT(str1, HasSubstr("Thanks for"));
}

TEST(MatchersTest, StringEquivalence) {
  auto str1 = std::string("aaaa");
  auto str2 = std::string("AAAA");
  auto str3 = std::string("ccccdddd");

  EXPECT_THAT(str1, StrEq(str1));
  EXPECT_THAT(str1, Eq(str1));
  EXPECT_THAT(str1, str1);

  EXPECT_THAT(str1, StrNe(str2));
  EXPECT_THAT(str1, Ne(str2));
  EXPECT_THAT(str1, Not(Eq(str2)));
  EXPECT_THAT(str1, Not(str2));

  EXPECT_THAT(str1, StrCaseEq(str2));
  EXPECT_THAT(str1, StrCaseNe(str3));
}
