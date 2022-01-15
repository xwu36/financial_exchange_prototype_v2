#include <algorithm>
#include <iostream>
#include <list>
#include <random>
#include <set>
#include <tuple>
#include <vector>

#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::_;
using ::testing::AllOf;
using ::testing::AnyOf;
using ::testing::ContainerEq;
using ::testing::Contains;
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::FieldsAre;
using ::testing::Gt;

using ::testing::Ge;
using ::testing::HasSubstr;
using ::testing::IsSubsetOf;
using ::testing::Lt;
using ::testing::Matcher;
using ::testing::Not;
using ::testing::Pair;
using ::testing::Pointwise;
using ::testing::PrintToString;
using ::testing::Truly;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
using ::testing::WhenSorted;

//-----------------------------------------------------------------------------
// Custom Matchers
//-----------------------------------------------------------------------------
MATCHER(IsEven, "") { return (arg % 2) == 0; }
MATCHER(IsEvenBtwIUnderstandCamelNotation, "") { return (arg % 2) == 0; }

MATCHER(IsOdd, "") { return (arg % 2) != 0; }

MATCHER_P(IsDivisibleBy, n, "") {
  *result_listener << "where the remainder is " << (arg % n);
  return (arg % n) == 0;
}

MATCHER_P2(IsBetween, a, b,
           (std::string(negation ? "Isn't" : "Is") + " between " +
            PrintToString(a) + " and " + PrintToString(b))) {
  return a <= arg && arg <= b;
}

MATCHER_P3(IsOneOf, a, b, c, "") { 
  return arg == a || arg == b || arg == c; 
}
//-----------------------------------------------------------------------------

TEST(MatchersTest, CustomMatchers) {
  EXPECT_THAT(10, IsEven());
  EXPECT_THAT(9, Not(IsEven()));
}


TEST(MatchersTest, CustomMatchers_Camel) {
  EXPECT_THAT(8, IsEvenBtwIUnderstandCamelNotation());
  EXPECT_THAT(9, Not(IsEvenBtwIUnderstandCamelNotation()));
}


TEST(MatchersTest, CustomMatchers_WithDescription) {
  EXPECT_THAT(10, IsBetween(9, 11));
  EXPECT_THAT(100, Not(IsBetween(9, 11)));
}

TEST(MatchersTest, CustomMatchers_ExtraDescription) {
  EXPECT_THAT(10, IsDivisibleBy(2));
  EXPECT_THAT(9, Not(IsDivisibleBy(2)));

  std::vector<int> v = {4, 6, 8, 2};
  EXPECT_THAT(v, Each(IsDivisibleBy(2)));
}

TEST(MatchersTest, CustomMatchers_Composite) {
  std::vector<int> v = {4, 6, 8, 2};

  EXPECT_THAT(v, Each(IsDivisibleBy(2)));

  EXPECT_THAT(
      v, ElementsAre(Lt(5), IsEven(), IsBetween(1, 10), Not(IsBetween(5, 10))));

  EXPECT_THAT(v, Each(Not(IsBetween(-1, -2))));
}

TEST(MatchersTest, CustomMatchers_ThreeParameters) {
  EXPECT_THAT(2 + 2, IsOneOf(2, 3, 4));
}