// Filtering tests.
// Use either of these commands to run:
// bazel test tests/gtest_demo:filtered_tests --test_arg=--gtest_filter=*.My*
// OR
// bazel test tests/gtest_demo:filtered_tests --test_filter=*.My*

#include <iostream>
#include <string>

#include "gtest/gtest.h"
int a = 1;
int b = 1;
//-----------------------------------------------------------------------------
TEST(BasicChecks, MyTest1) { EXPECT_TRUE(true); }

TEST(BasicChecks, MyTest2) { EXPECT_TRUE(true); }

TEST(BasicChecks, YourTest1) { EXPECT_TRUE(false); }

TEST(BasicChecks, YourTest2) { EXPECT_TRUE(false); }
