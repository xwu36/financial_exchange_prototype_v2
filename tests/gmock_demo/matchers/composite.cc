#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::_;
using ::testing::AllOf;
using ::testing::AllOfArray;
using ::testing::AnyOf;
using ::testing::Conditional;
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
using ::testing::Matcher;
using ::testing::MatchesRegex;
using ::testing::Not;
using ::testing::Pair;
using ::testing::StartsWith;
using ::testing::Truly;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
using ::testing::WhenSorted;

//-----------------------------------------------------------------------------
TEST(MatchersTest, CompositMatchersSimple) {
  int a = 1;
  EXPECT_THAT(a, AllOf(Eq(1), Gt(0), Lt(5)));
  
  EXPECT_THAT(a, Not(AllOf(Eq(1), Lt(0), Gt(5))));
  EXPECT_THAT(a, AnyOf(Eq(1), Lt(0), Gt(5)));
}

TEST(MatchersTest, CompositMatchers) {
  auto str = std::string("This is aa test. Thanks for watching.");
  EXPECT_THAT(str, AllOf(HasSubstr("aa"), HasSubstr("test")));
  EXPECT_THAT(str, Not(AllOf(HasSubstr("aa"), HasSubstr("bb"))));
  EXPECT_THAT(str, AnyOf(HasSubstr("aa"), HasSubstr("bb")));
  std::vector<Matcher<std::string>> matchers = {EndsWith("watching."),
                                                HasSubstr("Thanks for")};
  EXPECT_THAT(str, AllOfArray(matchers));

  bool selector = true;
  EXPECT_THAT(str, Conditional(selector, HasSubstr("aa"), HasSubstr("bb")));
}
