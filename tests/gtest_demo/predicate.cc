
#include <string>

#include "gtest/gtest.h"

bool IsAlmostEq(int a, int b) {
  if (std::abs(a - b) < 10)
    return true;
  else
    return false;
}

testing::AssertionResult IsAlmostEq2(int a, int b) {
  if (std::abs(a - b) < 10) {
    return testing::AssertionSuccess();
  } else {
    return testing::AssertionFailure()
           << "a is " << a << " and b is: " << b
           << ". The difference is: " << std::abs(a - b);
  }
}

TEST(ExpectPredTest, IsAlmostEq) {
  int a = 10;
  int b = 1000;
  EXPECT_TRUE(IsAlmostEq(a, b));
  EXPECT_PRED2(IsAlmostEq, a, b);
}

TEST(ExpectPredTest, IsAlmostEq2) {
  int a = 10;
  int b = 1000;
  EXPECT_TRUE(IsAlmostEq2(a, b));
}
