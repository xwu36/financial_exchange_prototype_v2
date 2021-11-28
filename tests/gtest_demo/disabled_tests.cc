
#include <iostream>
#include <string>

#include "gtest/gtest.h"

//-----------------------------------------------------------------------------
TEST(BasicChecks, DISABLED_SomeBrokenTest) {
  EXPECT_TRUE(false);
  EXPECT_TRUE(false);
}

TEST(BasicChecks, EnabledTest) {
  EXPECT_TRUE(true);
  EXPECT_TRUE(true);
}

TEST(BasicChecks, EnabledButSkippedTest) {
  GTEST_SKIP() << "Skipping single test";
  EXPECT_TRUE(false);
  EXPECT_TRUE(false);
}
//-----------------------------------------------------------------------------
class DISABLED_BasicChecks : public testing::Test {};
TEST_F(DISABLED_BasicChecks, SomeTest1) { EXPECT_TRUE(false); }
TEST_F(DISABLED_BasicChecks, SomeTest2) { EXPECT_TRUE(false); }
//-----------------------------------------------------------------------------
