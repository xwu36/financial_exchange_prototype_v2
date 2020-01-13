#include "src/lib/solution.h"
#include "gtest/gtest.h"
#include <vector>

TEST(HelloWorldShould, ReturnHelloWorld) {
  Solution solution;
  std::string actual = solution.PrintHelloWorld();
  std::string expected = "**** Hello World ****";
  EXPECT_EQ(expected, actual);
}

TEST(HelloWorldShould, HandleVectorSize) {
  Solution solution;
  std::vector<int> inputs = {1, 2, 3};
  auto actual = solution.ReturnsSizeOfVector(inputs);
  EXPECT_EQ(2, actual)
      << "ERROR: Expected the vector size to be 2, but it wasn't!";
}