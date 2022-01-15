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
using ::testing::Ge;
using ::testing::Gt;
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
using ::testing::WhenSortedBy;

//-----------------------------------------------------------------------------
TEST(MatchersTest, ElementsAre) {
  std::vector<int> v = {3, 4, 5, 2, 6, 7, 8, 9, 10};

  EXPECT_THAT(v, ElementsAre(3, 4, 5, 2, 6, 7, 8, 9, 10));
}

TEST(MatchersTest, WhenSorted) {
  std::vector<int> v = {3, 4, 5, 2, 6, 7, 8, 9, 10};
  EXPECT_THAT(v, WhenSorted(ElementsAre(2, 3, 4, 5, 6, 7, 8, 9, 10)));
  EXPECT_THAT(v, WhenSortedBy(std::greater<int>(),
                              ElementsAre(10, 9, 8, 7, 6, 5, 4, 3, 2)));
}

TEST(MatchersTest, UnorderedElementsAre) {
  std::vector<int> v = {3, 4, 5, 2};
  EXPECT_THAT(v, UnorderedElementsAre(4, 3, 5, 2));
}

TEST(MatchersTest, UnorderedElementsAreArray) {
  std::vector<int> v = {3, 4, 5, 2, 6, 7, 8, 9, 10};
  EXPECT_THAT(v, UnorderedElementsAreArray({2, 3, 4, 5, 6, 7, 8, 9, 10}));
  EXPECT_THAT(v, UnorderedElementsAreArray(v.begin(), v.end()));
}

TEST(MatchersTest, UnorderedElementsAreArray_Set) {
  std::vector<int> v = {3, 4, 5, 2, 6, 7, 8, 9, 10};
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

TEST(MatchersTest, ContainerMatchers_ContainerEq) {
  std::vector<int> v1{1, 2, 3, 4, 5};
  std::vector<int> v2{1, 2, 3};

  EXPECT_THAT(v1, Not(v2));
  EXPECT_THAT(v1, Not(ContainerEq(v2)));
}
