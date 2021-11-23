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

//-----------------------------------------------------------------------------
TEST(MatchersTest, NoMatcherIsUSed) {
  std::string test_string("Hello, this is a test string. We are here to help!");
  EXPECT_TRUE(test_string.rfind("Hello", 0) == 0);
  EXPECT_TRUE(std::regex_match(test_string, std::regex(".*is.*help.*")));

  int test_value = 10;
  EXPECT_GT(test_value, 0);
  EXPECT_LT(test_value, 100);
}

TEST(MatchersTest, MatchersAreUsed) {
  std::string test_string("Hello, this is a test string. We are here to help!");
  EXPECT_THAT(test_string, StartsWith("Hello"));
  EXPECT_THAT(test_string, MatchesRegex(".*is.*help.*"));

  int test_value = 10;
  ASSERT_THAT(test_value, AllOf(Gt(0), Lt(100)));
}
