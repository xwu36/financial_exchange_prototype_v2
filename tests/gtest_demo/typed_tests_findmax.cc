#include <vector>

#include "gtest/gtest.h"

class MyLib {
 public:
  virtual ~MyLib() {}
  // Finds the max element in a vector
  virtual int FindMax(const std::vector<int>& inputs) const = 0;
};

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

// A test fixture class template.
template <class T>
class MyLibTest : public testing::Test {
 protected:
  MyLibTest() : lib_(new T()) {}

  ~MyLibTest() override { delete lib_; }

  MyLib* const lib_;
};

using testing::Types;

// The list of types we want to test.
typedef Types<MyLibIterative, MyLibRecursive> Implementations;

// Then use TYPED_TEST(TestCaseName, TestName) to define a typed test,
TYPED_TEST_SUITE(MyLibTest, Implementations);

// similar to TEST_F.
// Since we are in the template world, C++ requires explicitly
// writing 'this->' when referring to members of the fixture class.
// This is something you have to learn to live with.

TYPED_TEST(MyLibTest, FindMaxHandlesEmpty) {
  std::vector<int> inputs = {};
  EXPECT_EQ(this->lib_->FindMax(inputs), -1);
}

TYPED_TEST(MyLibTest, FindMaxHandlesPositiveSizeOne) {
  std::vector<int> inputs = {2};
  EXPECT_EQ(this->lib_->FindMax(inputs), 2);
}

TYPED_TEST(MyLibTest, FindMaxHandlesConsecutiveNumbers) {
  std::vector<int> inputs = {1, 2, 3, 4};
  EXPECT_EQ(this->lib_->FindMax(inputs), 4);
}

TYPED_TEST(MyLibTest, FindMaxHandlesNonConsecutiveNumbers) {
  std::vector<int> inputs = {1, 12, 8, 22, 2, 18, 3, 1, 4};
  EXPECT_EQ(this->lib_->FindMax(inputs), 22);
}

TYPED_TEST(MyLibTest, FindMaxHandlesAllDuplicateNumbers) {
  std::vector<int> inputs = {1, 1, 1, 1, 1, 1};
  EXPECT_EQ(this->lib_->FindMax(inputs), 1);
}
