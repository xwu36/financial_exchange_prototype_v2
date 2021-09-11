#include <map>
#include <vector>

#include "gtest/gtest.h"

// Calculates the Nth Fibonacci number.
// Returns a negative value if the input is negative.
int Fib(int n) {
  if (n <= 1) {
    return n;
  }

  return Fib(n - 1) + Fib(n - 2);
}

TEST(FibTest, NonNegativeValues) {
  EXPECT_EQ(Fib(0), 0);
  EXPECT_EQ(Fib(1), 1);
  EXPECT_EQ(Fib(5), 5);
  EXPECT_EQ(Fib(6), 8) << "Error: The factorial calculation was wrong!";
}

TEST(FibTest, NegativeValues) {
  EXPECT_LT(Fib(-1), 0);
  EXPECT_LT(Fib(-10), 0);
}
