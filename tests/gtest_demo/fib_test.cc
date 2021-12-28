#include <map>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::Lt;

// Calculates the n'th Fib number.
// Returns a negative value if the input is negative.
int Fib(int n) {
  if (n <= 1) {
    return n;
  }

  return Fib(n - 1) + Fib(n - 2);
}

TEST(FibTest, NonNegativeValues) {
  std::cout << "Fib(5)= " << Fib(5) << std::endl;
  EXPECT_EQ(Fib(0), 0);
  EXPECT_EQ(Fib(1), 1);
  EXPECT_EQ(Fib(5), 5);
  EXPECT_EQ(Fib(6), 8) << "Error: The factorial calculation was wrong!";
}

TEST(FibTest, BasicTest) {
  EXPECT_EQ(Fib(0), 0);
  EXPECT_EQ(Fib(1), 1);
  EXPECT_EQ(Fib(2), 1);
  EXPECT_EQ(Fib(3), 2);
  EXPECT_EQ(Fib(4), 3);
}

TEST(FibTest, NegativeValues) {
  EXPECT_LT(Fib(-1), 0);
  EXPECT_LT(Fib(-10), 0);
  EXPECT_THAT(Fib(-10), Lt(0));
}
