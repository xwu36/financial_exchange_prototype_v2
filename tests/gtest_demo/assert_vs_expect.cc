
#include <iostream>
#include <string>

#include "gtest/gtest.h"

TEST(AssertAndExpect, AssertFailIsFatal) {
  ASSERT_TRUE(false);
  std::cout << "This line won't be executed!" << std::endl;
  EXPECT_TRUE(false);
}

TEST(AssertAndExpect, ExpectFailIsNotFatal) {
  EXPECT_TRUE(false);
  std::cout << "This line will be executed!" << std::endl;
  EXPECT_TRUE(false);
}