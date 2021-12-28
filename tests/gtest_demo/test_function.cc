
#include <functional>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

template <class T>
void Swap(T &i, T &j) {
  T temp = i;
  i = j;
  j = temp;
}

int FindMinIndex(const std::vector<int> &input, int start_index) {
  int min_index = start_index;
  int cur_min = input[start_index];
  for (size_t i = start_index; i < input.size(); i++) {
    if (input[i] < cur_min) {
      cur_min = input[i];
      min_index = i;
    }
  }
  return min_index;
}
void SelectionSort(std::vector<int> &input) {
  for (int i = 0; i < int(input.size() - 1); i++) {
    int min_index = FindMinIndex(input, i);
    Swap(input[i], input[min_index]);
  }
}
void BubbleSort(std::vector<int> &input) {
  bool go;
  do {
    go = false;
    for (int i = 0; i < int(input.size() - 1); i++) {
      if (input[i] > input[i + 1]) {
        Swap(input[i], input[i + 1]);
        go = true;
      }
    }
  } while (go);
}
//-----------------------------------------------------------------------------
// Method 1: Function Pointer.
//-----------------------------------------------------------------------------
template <class T>
void TestSort(T sort_func) {
  std::vector<int> in;
  std::vector<int> expected;

  in = {1, 11, 2, 5, 12, 9, 4, 10};
  sort_func(in);
  expected = {1, 2, 4, 5, 9, 10, 11, 12};
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());

  // Check empty
  in = {};
  sort_func(in);
  expected = {};
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());

  // Sort array of 1 element
  in = {3};
  sort_func(in);
  expected = {3};
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());

  // Check small vector
  in = {5, 3, 1, 77, -1};
  sort_func(in);
  expected = {-1, 1, 3, 5, 77};
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());

  // Reverse order
  in = {5, 4, 3, 2, 1};
  sort_func(in);
  expected = {1, 2, 3, 4, 5};
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());

  in = {5, 4, 3, 2, 1, 0};
  sort_func(in);
  expected = {0, 1, 2, 3, 4, 5};
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());

  // With duplicate values
  in = {-4, 122, -1000, 222, 45,  66,  97,    1,
        23, 44,  23,    100, 244, 456, -1000, 22};
  sort_func(in);
  expected = {-1000, -1000, -4, 1,   22,  23,  23,  44,
              45,    66,    97, 100, 122, 222, 244, 456};
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());

  // Random large vector, testing with multiple seeds
  for (size_t i = 0; i < 50; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(1000);
    std::generate(in.begin(), in.end(), std::rand);
    expected = in;
    std::sort(expected.begin(), expected.end());
    sort_func(in);
    EXPECT_EQ(expected, in);
    EXPECT_EQ(expected.size(), in.size());
  }

  //  large vector sorted
  for (size_t i = 0; i < 50; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(1000);
    std::generate(in.begin(), in.end(), std::rand);
    std::sort(in.begin(), in.end());
    expected = in;
    std::sort(expected.begin(), expected.end());
    sort_func(in);
    EXPECT_EQ(expected, in);
    EXPECT_EQ(expected.size(), in.size());
  }

  //  large vector reverse sorted
  for (size_t i = 0; i < 50; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(1000);
    std::generate(in.begin(), in.end(), std::rand);
    std::sort(in.begin(), in.end(), std::greater<int>());
    expected = in;
    std::sort(expected.begin(), expected.end());
    sort_func(in);
    EXPECT_EQ(expected, in);
    EXPECT_EQ(expected.size(), in.size());
  }

  // Random larger vector, testing with multiple seeds
  for (size_t i = 0; i < 5; i++) {
    std::srand(i);  // use a constant seed to make the test repeatable
    in.resize(5000);
    std::generate(in.begin(), in.end(), std::rand);
    expected = in;
    std::sort(expected.begin(), expected.end());
    sort_func(in);
    EXPECT_EQ(expected, in);
    EXPECT_EQ(expected.size(), in.size());
  }

  //  large vector reverse sorted
  for (size_t i = 0; i < 5; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(5000);
    std::generate(in.begin(), in.end(), std::rand);
    std::sort(in.begin(), in.end(), std::greater<int>());
    expected = in;
    std::sort(expected.begin(), expected.end());
    sort_func(in);
    EXPECT_EQ(expected, in);
    EXPECT_EQ(expected.size(), in.size());
  }
}

TEST(SortTest, SelectionSort) { TestSort(SelectionSort); }
TEST(SortTest, BubbleSort) { TestSort(BubbleSort); }
//-----------------------------------------------------------------------------
// Method 2: Everything in one test.
//-----------------------------------------------------------------------------
TEST(SortTestAllAtOnce, WorksOnAllFunctionsWithAllInputs) {
  std::vector<int> in;
  std::vector<std::vector<int>> ins = {
      {},                         // empty
      {1},                        // single size
      {5, 3, 1, 77},              // small size random values
      {5, 4, 3, 2, 1},            // reversely sorted
      {-4, 122, -1000, -4, 122},  // with duplicates
      {-4, 122, -1000, 222, 45, 66, 97, 1, 23, 44, 23, 100, 244, 456, -1000,
       22}};

  // Random large vector, testing with multiple seeds
  for (size_t i = 0; i < 50; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(1000);
    std::generate(in.begin(), in.end(), std::rand);
    ins.push_back(in);
  }

  //  large vector sorted
  for (size_t i = 0; i < 50; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(1000);
    std::generate(in.begin(), in.end(), std::rand);
    ins.push_back(in);
  }

  //  large vector reverse sorted
  for (size_t i = 0; i < 50; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(1000);
    std::generate(in.begin(), in.end(), std::rand);
    ins.push_back(in);
  }

  // Random larger vector, testing with multiple seeds
  for (size_t i = 0; i < 5; i++) {
    std::srand(i);  // use a constant seed to make the test repeatable
    in.resize(5000);
    std::generate(in.begin(), in.end(), std::rand);
    ins.push_back(in);
  }

  //  large vector reverse sorted
  for (size_t i = 0; i < 5; i++) {
    std::srand(i * 2);  // use a constant seed to make the test repeatable
    in.resize(5000);
    std::generate(in.begin(), in.end(), std::rand);
    ins.push_back(in);
  }

  std::vector<std::function<void(std::vector<int> &)>> functions = {
      SelectionSort, BubbleSort};

  // Applying all inputs and all functions at once!
  for (auto f : functions) {
    for (auto &in : ins) {
      auto expected = in;
      f(in);
      std::sort(expected.begin(), expected.end());
      EXPECT_EQ(expected, in);
    }
  }
}
