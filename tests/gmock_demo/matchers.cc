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
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::FieldsAre;
using ::testing::Gt;
using ::testing::IsSubsetOf;
using ::testing::Lt;
using ::testing::Not;
using ::testing::Pair;
using ::testing::Truly;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
using ::testing::WhenSorted;
using ::testing::ContainsRegex;

//-----------------------------------------------------------------------------
TEST(MatchersTest, ElementsAre) {
  auto str = std::string("This is a test. Thanks for watching.");

  EXPECT_THAT(str, ContainsRegex(".*a test\\."));
}
