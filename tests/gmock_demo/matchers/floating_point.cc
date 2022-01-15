#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
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
using ::testing::DoubleEq;
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::FieldsAre;
using ::testing::FloatEq;
using ::testing::FloatNear;
using ::testing::Gt;
using ::testing::HasSubstr;
using ::testing::IsNan;
using ::testing::IsSubsetOf;
using ::testing::Lt;
using ::testing::MatchesRegex;
using ::testing::NanSensitiveFloatEq;
using ::testing::Not;
using ::testing::Pair;
using ::testing::StartsWith;
using ::testing::Truly;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
//-----------------------------------------------------------------------------

const float a = 67329.234;
const float b = 67329.242;  // Exactly 1 ULP away from a
//-----------------------------------------------------------------------------
TEST(FloatingPointTest, ULPComparison) {
  EXPECT_NE(a, b);        // Exactly 1 ULP away.
  EXPECT_FLOAT_EQ(a, b);  // Exactly 1 ULP away, Passes
  EXPECT_THAT(a, FloatEq(b));
}

TEST(FloatingPointTest, FloatNear) {
  EXPECT_NEAR(a, b, 0.2);  // 0.2 Difference
  EXPECT_THAT(a, FloatNear(b, 0.2));
}

TEST(FloatingPointTest, NanSensitiveFloatEq) {
  auto qNaN = std::numeric_limits<float>::quiet_NaN();
  auto logNaN = std::log(-1);

  EXPECT_THAT(qNaN, IsNan());
  EXPECT_THAT(logNaN, IsNan());
  
  EXPECT_THAT(qNaN, NanSensitiveFloatEq(logNaN));
  EXPECT_THAT(qNaN, Not(FloatEq(logNaN)));
}
