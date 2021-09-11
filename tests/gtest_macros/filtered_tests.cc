
#include <iostream>
#include <string>

#include "gtest/gtest.h"
int a = 0;
int b = 1;
//-----------------------------------------------------------------------------
TEST(BasicChecks, MyTest1) {
  EXPECT_TRUE(0 == 0);
}

TEST(BasicChecks, YourTest1) {
  EXPECT_TRUE(1 == 0);
}

TEST(BasicChecks, MyTest2) {
  EXPECT_TRUE(1 == 1);
}

TEST(BasicChecks, YourTest2) {
  EXPECT_TRUE(1 == 2);
}
