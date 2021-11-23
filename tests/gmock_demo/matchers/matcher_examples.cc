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
//-----------------------------------------------------------------------------

// Returns non-zero if the input is positive.
bool IsPositive(int x) { return x > 0; }

bool IsWhole(double x) { return std::floor(x) == x; }

//-----------------------------------------------------------------------------
TEST(MatchersTest, ElementsAre) {
  std::vector<int> v = {3, 4, 5, 2, 6, 7, 8, 9, 10};

  EXPECT_THAT(v, ElementsAre(3, 4, 5, 2, 6, 7, 8, 9, 10));
  EXPECT_THAT(v, ElementsAreArray({3, 4, 5, 2, 6, 7, 8, 9, 10}));
}

TEST(MatchersTest, WhenSorted) {
  std::vector<int> v = {3, 4, 5, 2, 6, 7, 8, 9, 10};
  EXPECT_THAT(v, WhenSorted(ElementsAre(2, 3, 4, 5, 6, 7, 8, 9, 10)));
  EXPECT_THAT(v, UnorderedElementsAre(2, 3, 4, 5, 6, 7, 8, 9, 10));
}

TEST(MatchersTest, UnorderedElementsAre) {
  std::vector<int> v = {3, 4, 5, 2};
  EXPECT_THAT(v, UnorderedElementsAre(4, 3, 5, 2));
}

TEST(MatchersTest, UnorderedElementsAreArray) {
  std::vector<int> v = {3, 4, 5, 2, 6, 7, 8, 9, 10};
  EXPECT_THAT(v, UnorderedElementsAreArray({2, 3, 4, 5, 6, 7, 8, 9, 10}));
  EXPECT_THAT(v, UnorderedElementsAreArray(v.begin(), v.end()));

  std::set<int> s(v.begin(), v.end());

  EXPECT_THAT(v, UnorderedElementsAreArray(s.begin(), s.end()));
}

TEST(MatchersTest, ContainerMatchers) {
  std::vector<int> v = {4, 6, 8, 2, 2};
  std::vector<int> u = {4, 6};

  EXPECT_THAT(v, Each(Gt(1)));

  EXPECT_THAT(v, Contains(6));

  EXPECT_THAT(u, IsSubsetOf(v));

  // Contains an item that is greater than 1.
  EXPECT_THAT(v, Contains(Gt(1)));

  // Each element is either greater than 2 or greater than 0
  EXPECT_THAT(v, Each(AnyOf(Gt(2), Gt(0))));
}

TEST(MatchersTest, ContainerMatchers_ContainsNTimes) {
  std::vector<int> v = {4, 6, 8, 2, 2, 3, 3, 3, 4};

  EXPECT_THAT(v, Contains(3).Times(3));
  EXPECT_THAT(v, Contains(4).Times(2));
  EXPECT_THAT(v, Contains(1).Times(0));
}

TEST(MatchersTest, ContainerMatchers_Pointwise) {
  std::list<int> lhs{1, 2, 3};
  std::vector<int> rhs{4, 5, 6};

  EXPECT_THAT(lhs, Pointwise(Lt(), rhs));
  EXPECT_THAT(lhs, Not(Pointwise(Gt(), rhs)));
}

TEST(MatchersTest, CustomMatchers) {
  EXPECT_THAT(10, IsEven());
  EXPECT_THAT(9, Not(IsEven()));
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

TEST(MatchersTest, CustomMatchers3) {
  std::vector<int> v = {4, 6, 8, 2};

  EXPECT_THAT(v, Each(Gt(1)));

  // Each element is either greater than 2 or greater than 0
  EXPECT_THAT(v, Each(AnyOf(Gt(2), Gt(0))));

  EXPECT_THAT(v, Each(IsDivisibleBy(2)));

  EXPECT_THAT(
      v, ElementsAre(Lt(5), IsEven(), IsBetween(1, 10), Not(IsBetween(5, 10))));

  EXPECT_THAT(v, Each(Not(IsBetween(-1, -2))));
}

TEST(MatchersTest, Truly) {
  std::vector<double> v = {4, 6, 8, 2};
  EXPECT_THAT(v, Each(Truly(IsPositive)));
  EXPECT_THAT(v, Each(Truly(IsWhole)));
}

TEST(MatchersTest, IsSubsetOf) {
  std::vector<int> subset{1};
  std::vector<int> superset{2, 1};
  std::vector<int> superset_with_duplicates{2, 2, 1};
  std::vector<int> disjoint{4, 5};

  EXPECT_THAT(subset, IsSubsetOf(superset));
  EXPECT_THAT(subset, IsSubsetOf(superset_with_duplicates));
  EXPECT_THAT(subset, Not(IsSubsetOf(disjoint)));
}

TEST(MatchersTest, UnorderedElementsAreArray_duplicate) {
  std::vector<int> v = {1, 1, 2, 2, 3, 4, 5, 6};
  std::set<int> s(v.begin(), v.end());

  std::random_device rd;
  std::mt19937 g(rd());

  std::shuffle(v.begin(), v.end(), g);

  EXPECT_THAT(s, IsSubsetOf(v));
}

TEST(MatchersTest, MemberMatchers) {
  std::map<int, int> m = {{1, 100}, {2, 200}, {3, 300}};

  // Each pair is not equal to (1,100).
  EXPECT_THAT(m, Not(Each(Pair(1, 100))));

  // The second part of each pair is greater than 99.
  EXPECT_THAT(m, Each(Pair(_, Gt(99))));

  // The first part of each pair is greater than 0 and the second part is either
  // greater than 100 or equal to 100.
  EXPECT_THAT(m, Each(FieldsAre(Gt(0), AnyOf(Gt(100), Eq(100)))));
}

TEST(MatchersTest, MemberMatchers_Field_Tuple) {
  std::tuple<int, std::string> my_tuple{7, "hello world"};
  EXPECT_THAT(my_tuple, FieldsAre(Ge(0), HasSubstr("hello")));
}

// Requires C++17
TEST(MatchersTest, MemberMatchers_Field_Struct) {
  struct MyStruct {
    int value = 42;
    std::string greeting = "aloha";
  };
  MyStruct s;
  EXPECT_THAT(s, FieldsAre(42, "aloha"));
}
