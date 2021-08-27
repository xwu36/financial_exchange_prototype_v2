#include <map>
#include <vector>

#include "gtest/gtest.h"

// Calculates the Nth Fibonacci number.
int Fib(int n) {
  if (n <= 1) {
    return n;
  }

  return Fib(n - 1) + Fib(n - 2);
}

// Finds the maximum value in a vector.
int FindMax(const std::vector<int> &inputs) {
  if (inputs.size() == 0) {
    return -1;
  }

  int result = std::numeric_limits<int>::min();
  for (auto n : inputs) {
    if (n > result) {
      result = n;
    }
  }
  return result;
}

TEST(FibTest, CalculateFibonacci) {
  EXPECT_EQ(Fib(1), 1);
  EXPECT_EQ(Fib(5), 5) << "Error: The factorial calculation was wrong!";
  EXPECT_EQ(Fib(6), 8) << "Error: The factorial calculation was wrong!";
}

TEST(CPPLibTest, CalculateFibonacciNegativeInput) {
  EXPECT_EQ(Fib(-1), -1);
  EXPECT_LT(Fib(-10), 0);
}

TEST(CPPLibTest, CalculateFibonacciOnVector) {
  std::vector<int> inputs = {1, 2, 3, 4, 5, 6};
  std::vector<int> actual;

  for (auto &in : inputs) {
    actual.push_back(Fib(in));
  }

  std::vector<int> expected = {1, 1, 2, 3, 5, 8};
  EXPECT_EQ(expected, actual) << "Error: The factorial calculation was wrong !";
}

//-----------------------------------------------------------------------------
TEST(CPPLibTest, FindMaxHandlesSizeOne) {
  std::vector<int> inputs = {2};
  EXPECT_EQ(FindMax(inputs), 2);
}

TEST(CPPLibTest, FindMaxHandlesConsecutiveNumbers) {
  std::vector<int> inputs = {1, 2, 3, 4};
  EXPECT_EQ(FindMax(inputs), 4);
}

TEST(CPPLibTest, FindMaxHandlesEmptyVector) {
  std::vector<int> inputs = {};
  EXPECT_EQ(FindMax(inputs), -1);
}