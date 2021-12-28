#include <vector>

#include "gtest/gtest.h"

// An abstract interface
class MyLib {
 public:
  virtual ~MyLib() {}
  // Finds the max element in a vector
  virtual int FindMax(const std::vector<int>& inputs) const = 0;
};
//-----------------------------------------------------------------------------
// Iterative implementation.
class MyLibIterative : public MyLib {
 public:
  // Finds the max element in a vector
  virtual int FindMax(const std::vector<int>& inputs) const override {
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
};
//-----------------------------------------------------------------------------
// Recursive implementation.
class MyLibRecursive : public MyLib {
 public:
  // Finds the max element in a vector

  virtual int FindMax(const std::vector<int>& inputs) const override {
    if (inputs.size() == 0) {
      return -1;
    }
    return FindMaxRecursiveAux(inputs, 0, inputs.size() - 1);
  }

 private:
  int FindMaxRecursiveAux(const std::vector<int>& inputs, int left,
                          int right) const {
    if (left >= right) {
      return inputs[left];
    }
    int mid = (right + left) / 2;
    return std::max(FindMaxRecursiveAux(inputs, left, mid),
                    FindMaxRecursiveAux(inputs, mid + 1, right));
  }
};
//-----------------------------------------------------------------------------
// A test fixture class template.
template <class T>
class MyLibTest : public testing::Test {
 public:
  T lib_;
};

using testing::Types;

TYPED_TEST_SUITE_P(MyLibTest);

TYPED_TEST_P(MyLibTest, FindMaxHandlesEmpty) {
  std::vector<int> inputs = {};
  EXPECT_EQ(this->lib_.FindMax(inputs), -1);
}

TYPED_TEST_P(MyLibTest, FindMaxHandlesPositiveSizeOne) {
  std::vector<int> inputs = {2};
  EXPECT_EQ(this->lib_.FindMax(inputs), 2);
}

TYPED_TEST_P(MyLibTest, FindMaxHandlesConsecutiveNumbers) {
  std::vector<int> inputs = {1, 2, 3, 4};
  EXPECT_EQ(this->lib_.FindMax(inputs), 4);
}

TYPED_TEST_P(MyLibTest, FindMaxHandlesNonConsecutiveNumbers) {
  std::vector<int> inputs = {1, 12, 8, 22, 2, 18, 3, 1, 4};
  EXPECT_EQ(this->lib_.FindMax(inputs), 22);
}

TYPED_TEST_P(MyLibTest, FindMaxHandlesAllDuplicateNumbers) {
  std::vector<int> inputs = {1, 1, 1, 1, 1, 1};
  EXPECT_EQ(this->lib_.FindMax(inputs), 1);
}

REGISTER_TYPED_TEST_SUITE_P(MyLibTest, FindMaxHandlesEmpty,
                            FindMaxHandlesPositiveSizeOne,
                            FindMaxHandlesConsecutiveNumbers,
                            FindMaxHandlesNonConsecutiveNumbers,
                            FindMaxHandlesAllDuplicateNumbers);

// The list of types we want to test.  Note that it doesn't have to be
// defined at the time we write the TYPED_TEST_P()s.
typedef Types<MyLibIterative, MyLibRecursive> Implementations;

INSTANTIATE_TYPED_TEST_SUITE_P(IterativeAndRecursive,  // Instance name
                               MyLibTest,              // Test case name
                               Implementations);       // Type list