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

// Returns non-zero if the input is positive.
bool IsPositive(int x) { return x > 0; }

//-----------------------------------------------------------------------------

TEST(MatchersTest, WithoutTruly) {
  std::vector<double> v = {4, 6, 8, 2};

  for (auto& e : v) {
    EXPECT_TRUE(IsPositive(e));
    EXPECT_PRED1(IsPositive, e);
  }
}

TEST(MatchersTest, WithTruly) {
  std::vector<double> v = {4, 6, 8, 2};
  EXPECT_THAT(v, Each(Truly(IsPositive)));
}
//-----------------------------------------------------------------------------
TEST(ExpectPredTest, IsPositive) {
  EXPECT_TRUE(IsPositive(10));
  EXPECT_PRED1(IsPositive, 10);
}

TEST(ExpectPredTest, IsPositiveTruly) { EXPECT_THAT(10, Truly(IsPositive)); }
