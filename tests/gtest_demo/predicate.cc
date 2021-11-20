
#include <string>

#include "gtest/gtest.h"

bool IsAlmostEq(int a, int b) {
  if (std::abs(a - b) < 10)
    return true;
  else
    return false;
}

TEST(ExpectPredTest, IsAlmostEq) {
  int a = 0;
  int b = 1;
  EXPECT_TRUE(IsAlmostEq(a, b));
  EXPECT_PRED2(IsAlmostEq, a, b);
}
