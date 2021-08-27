
#include <iostream>
#include <string>

#include "gtest/gtest.h"
//-----------------------------------------------------------------------------
bool IsEq(int a, int b) {
  if (a == b)
    return true;
  else
    return false;
}
//-----------------------------------------------------------------------------
int increment(int a) { return ++a; }
//-----------------------------------------------------------------------------
TEST(BasicChecks, FalseAndTrue) {
  int a = 0;
  int b = 1;
  EXPECT_TRUE(a == 0);
  EXPECT_TRUE(b == 1);
  EXPECT_FALSE(a == b);
  EXPECT_FALSE(IsEq(a, b));
}

TEST(BasicChecks, SimpleAssertMacros) {
  int a = 0;
  int b = 1;
  ASSERT_EQ(a, a);
  ASSERT_NE(a, b);
  ASSERT_GT(b, a);
  ASSERT_GT(a, b);  // This will fail
  std::cout << ">--------------------After Assert-------------------->"
            << std::endl;
}
TEST(BasicChecks, SimpleExpectMacros) {
  int a = 0;
  int b = 1;
  EXPECT_EQ(a, a);
  EXPECT_NE(a, b);
  EXPECT_GT(b, a);
  EXPECT_GT(a, b);  // This will fail

  std::cout << ">--------------------After Expect-------------------->"
            << std::endl;
}

TEST(BasicChecks, ExplicitSuccessFails) {
  SUCCEED();
  // This will generate a nonfatal failure.
  ADD_FAILURE();
  // This will generate a nonfatal failure.
  ADD_FAILURE_AT("MyTestFile.cc", 10);

  // This will generate a fatal failure.
  FAIL();

  std::cout << ">--------------------After FAIL-------------------->"
            << std::endl;
}
