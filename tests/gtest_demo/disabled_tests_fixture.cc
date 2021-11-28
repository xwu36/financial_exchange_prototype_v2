
#include <iostream>
#include <string>

#include "gtest/gtest.h"

class SomeTextFixture : public ::testing::Test {
 protected:
  void SetUp() override {
    GTEST_SKIP() << "Skipping all tests for this fixture";
  }
};

// All tests in this fixture will be skipped.
TEST_F(SomeTextFixture, SomeTest1) { EXPECT_TRUE(false); }
TEST_F(SomeTextFixture, SomeTest2) { EXPECT_TRUE(false); }
