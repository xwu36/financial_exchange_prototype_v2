#include "src/lib/cpplib.h"

#include <map>
#include <vector>

#include "gtest/gtest.h"

TEST(CPPLibTest, ReturnHelloWorld) {
  CPPLib cpplib;
  std::string actual = cpplib.PrintHelloWorld();
  std::string expected = "**** Hello World ****";
  EXPECT_EQ(expected, actual);
}

TEST(CPPLibTest, CalculateFibonacci) {
  CPPLib cpplib;

  std::vector<int> inputs = {1, 2, 3, 4, 5, 6};
  std::vector<int> actual;

  for (auto &in : inputs) {
    actual.push_back(cpplib.fib(in));
  }

  std::vector<int> expected = {1, 1, 2, 3, 5, 8};
  EXPECT_EQ(expected, actual) << "Error: The factorial calculation was wrong!";
}

TEST(CPPLibTest, FindMaxHandlesSizeOne) {
  CPPLib cpplib;
  std::vector<int> inputs = {2};
  EXPECT_EQ(cpplib.FindMax(inputs), 2);
}

TEST(CPPLibTest, FindMaxHandlesConsecutiveNumbers) {
  CPPLib cpplib;
  std::vector<int> inputs = {1, 2, 3, 4};
  EXPECT_EQ(cpplib.FindMax(inputs), 4);
}

// TEST(CPPLibTest, FindMaxHandlesEmptyVector) {
//   CPPLib cpplib;
//   std::vector<int> inputs = {};
//   EXPECT_EQ(cpplib.FindMax(inputs), -1);
// }
