#include <algorithm>
#include <iostream>
#include <random>
#include <regex>
#include <set>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AllOf;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::MatchesRegex;
using ::testing::StartsWith;
using ::testing::Value;

//-----------------------------------------------------------------------------
TEST(MatchersTest, ExpectThat) {
  EXPECT_THAT(1, 1);
  EXPECT_THAT(2 - 2, 0);
  EXPECT_THAT(1 == 1, true);
}

TEST(MatchersTest, SimpleMatcher) {
  testing::Matcher<int> is_one = 1;
  testing::Matcher<int> is_zero = 0;
  testing::Matcher<bool> is_true = true;

  EXPECT_TRUE(is_one.Matches(1));
  EXPECT_TRUE(is_zero.Matches(2 - 2));
  EXPECT_TRUE(is_true.Matches(1 == 1));

  EXPECT_TRUE(Matches(is_one)(1));
  EXPECT_TRUE(Matches(is_zero)(0));
  EXPECT_TRUE(Matches(is_true)(true));
}

TEST(MatchersTest, SimpleMatcherExpectThat) {
  testing::Matcher<int> is_one = 1;
  testing::Matcher<int> is_zero = 0;
  testing::Matcher<bool> is_true = true;

  EXPECT_THAT(1, is_one);
  EXPECT_THAT(2 - 2, is_zero);
  EXPECT_THAT(1 == 1, is_true);
}

TEST(MatchersTest, ExpectEq) {
  EXPECT_EQ(1, 1);
  EXPECT_EQ(0, 2 - 2);
  EXPECT_TRUE(1 == 1);
}
//-----------------------------------------------------------------------------
TEST(MatchersTest, NoMatcherIsUSed) {
  std::string test_string("Hello, this is a test string. We are here to help!");
  EXPECT_TRUE(test_string.rfind("Hello", 0) == 0);
  EXPECT_TRUE(std::regex_match(test_string, std::regex(".*is.*help.*")));

  EXPECT_GT(10, 0);
  EXPECT_LT(10, 100);
}

TEST(MatchersTest, MatchersAreUsed_EXPECT_THAT) {
  std::string test_string("Hello, this is a test string. We are here to help!");
  EXPECT_THAT(test_string, StartsWith("Hello"));
  EXPECT_THAT(test_string, MatchesRegex(".*is.*help.*"));

  ASSERT_THAT(10, AllOf(Gt(0), Lt(100)));
}

TEST(MatchersTest, MatchersAreUsed) {
  std::string test_string("Hello, this is a test string. We are here to help!");
  testing::Matcher<std::string> m1 = StartsWith("Hello");
  testing::Matcher<std::string> m2 = MatchesRegex(".*is.*help.*");

  EXPECT_TRUE(m1.Matches(test_string));
  EXPECT_TRUE(m2.Matches(test_string));

  testing::Matcher<int> m3 = AllOf(Gt(0), Lt(100));
  ASSERT_TRUE(m3.Matches(10));
}

//-----------------------------------------------------------------------------
