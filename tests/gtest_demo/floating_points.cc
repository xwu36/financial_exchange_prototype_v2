
#include <iostream>
#include <string>

#include "gtest/gtest.h"

const float a = 67329.234;
const float b = 67329.242;  // Exactly 1 ULP away from a
//-----------------------------------------------------------------------------
TEST(FloatingPointTest, SimpleAssertion) {
  EXPECT_EQ(a, b);  // Exactly 1 ULP away, Fails!
}

TEST(FloatingPointTest, FpAssertion) {
  EXPECT_FLOAT_EQ(a, b);  // Exactly 1 ULP away, Passes
}

TEST(FloatingPointTest, FpAssertionNear) {
  EXPECT_NEAR((a - b), 0, 0.2);  // Exactly 1 ULP away, Passes
}
