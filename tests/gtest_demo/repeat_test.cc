// A test that uses SetUpTestSuite and TearDownTestSuite in combination with a
// global variable g_counter to run part of the code only in the first iteration
// of the test if --gtest_repeat is used.

#include <iostream>

#include "gtest/gtest.h"

bool g_first_iteration = true;

class FooTest : public testing::Test {
 protected:
  static void SetUpTestSuite() {
    std::cout << "========Beginning of all tests========" << std::endl;

    // This part is run only in the first iteration.
    if (g_first_iteration) {
      g_first_iteration = false;
      std::cout << "First time init" << std::endl;
    }
  }
};

TEST_F(FooTest, Test1) { EXPECT_EQ(1, 1); }