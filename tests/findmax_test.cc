#include <map>
#include <vector>

#include "gtest/gtest.h"

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

//-----------------------------------------------------------------------------
TEST(FindMaxTest, FindMaxHandlesSizeOne) {
  std::vector<int> inputs = {2};
  EXPECT_EQ(FindMax(inputs), 2);
}

TEST(FindMaxTest, FindMaxHandlesConsecutiveNumbers) {
  std::vector<int> inputs = {1, 2, 3, 4};
  EXPECT_EQ(FindMax(inputs), 4);
}

TEST(FindMaxTest, FindMaxHandlesEmptyVector) {
  std::vector<int> inputs = {};
  EXPECT_EQ(FindMax(inputs), -1);
}