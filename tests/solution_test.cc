#include "src/lib/solution.h"
#include "gtest/gtest.h"

TEST(HelloWorldShould, ReturnHelloWorld) {
  Solution solution;
  std::string actual = solution.PrintHelloWorld();
  std::string expected = "Hello World!";
  EXPECT_EQ(expected, actual);
}