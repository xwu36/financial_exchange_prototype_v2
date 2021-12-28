
#include <functional>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"

using ::testing::Combine;
using ::testing::Values;

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
    : public testing::TestWithParam<std::tuple<
          std::function<void(std::vector<int> &)>, std::vector<int>>> {};

INSTANTIATE_TEST_SUITE_P(SelectionSortBubbleSort, SortTest,
                         Combine(Values(SelectionSort, BubbleSort),
                                 Values(std::vector<int>{}, std::vector<int>{1},
                                        std::vector<int>{5, 3, 1, 77},
                                        std::vector<int>{5, 4, 3, 2, 1},
                                        std::vector<int>{-4, 122, -1000, -4,
                                                         122, -1000})));

TEST_P(SortTest, WorksForVariousInputs) {
  auto p = GetParam();

  auto in = std::get<1>(p);
  auto SortFunction = std::get<0>(p);
  auto expected = in;
  SortFunction(in);
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, in);
}
