
#include <iostream>
#include <string>

#include "gtest/gtest.h"
int a = 0;
int b = 1;
//-----------------------------------------------------------------------------
TEST(BasicChecks, DISABLED_SomeBrokenTest) {
  EXPECT_TRUE(a == 10);
  EXPECT_TRUE(b == 20);
}

TEST(BasicChecks, EnabledTest) {
  EXPECT_TRUE(a == 0);
  EXPECT_TRUE(b == 1);
}

TEST(BasicChecks, EnabledButSkippedTest) {
  GTEST_SKIP() << "Skipping single test";
  EXPECT_TRUE(a == 0);
  EXPECT_TRUE(b == 1);
}