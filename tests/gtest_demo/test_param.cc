
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

class SortTest
    : public testing::TestWithParam<std::function<void(std::vector<int> &)> > {
  // You can implement all the usual fixture class members here.
  // To access the test parameter, call GetParam() from class
  // TestWithParam<T>.
};

INSTANTIATE_TEST_SUITE_P(SelectionSortBubbleSort, SortTest,
                         testing::Values(SelectionSort, BubbleSort));

TEST_P(SortTest, SmallNumberOfItems) {
  std::vector<int> in = {1, 11, 2, 5, 12, 9, 4, 10};
  std::vector<int> expected = {1, 2, 4, 5, 9, 10, 11, 12};
  auto sort_func = GetParam();
  sort_func(in);
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());
}

TEST_P(SortTest, EmptyCollection) {
  std::vector<int> in = {};
  std::vector<int> expected = {};
  auto sort_func = GetParam();

  sort_func(in);
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());
}

TEST_P(SortTest, SingleElement) {
  std::vector<int> in = {3};
  std::vector<int> expected = {3};
  auto sort_func = GetParam();

  sort_func(in);
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());
}

TEST_P(SortTest, ReverseOrder) {
  std::vector<int> in = {5, 4, 3, 2, 1};
  std::vector<int> expected = {1, 2, 3, 4, 5};
  auto sort_func = GetParam();

  sort_func(in);
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());
}

TEST_P(SortTest, WithDuplicates) {
  std::vector<int> in = {-4, 122, -1000, 222, 45,  66,  97,    1,
                         23, 44,  23,    100, 244, 456, -1000, 22};
  std::vector<int> expected = {-1000, -1000, -4, 1,   22,  23,  23,  44,
                               45,    66,    97, 100, 122, 222, 244, 456};
  auto sort_func = GetParam();

  sort_func(in);
  EXPECT_EQ(expected, in);
  EXPECT_EQ(expected.size(), in.size());
}

TEST_P(SortTest, RandomLargeVector) {
  std::vector<int> in;
  std::vector<int> expected;
  auto sort_func = GetParam();

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
}

TEST_P(SortTest, RandomLargeVectorSorted) {
  std::vector<int> in;
  std::vector<int> expected;
  auto sort_func = GetParam();

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
}

TEST_P(SortTest, RandomLargeVectorReverseSorted) {
  std::vector<int> in;
  std::vector<int> expected;
  auto sort_func = GetParam();

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
}

TEST_P(SortTest, RandomLargeVectorMultipleSeeds) {
  std::vector<int> in;
  std::vector<int> expected;
  auto sort_func = GetParam();

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
}