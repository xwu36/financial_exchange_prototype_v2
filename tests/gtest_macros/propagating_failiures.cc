
#include <iostream>
#include <string>

#include "gtest/gtest.h"

void Subroutine() {
  // Generates a fatal failure and aborts the current function.
  ASSERT_EQ(1, 2);

  std::cout << "This line won't be executed!" << std::endl;
}

TEST(FooTest, Bar) {
  Subroutine();  // The intended behavior is for the fatal failure
                 // in Subroutine() to abort the entire test.

  EXPECT_EQ(3, 4);
}