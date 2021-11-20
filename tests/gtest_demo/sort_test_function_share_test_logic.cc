
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

//-----------------------------------------------------------------------------

TEST(SortTestSimple, SortsWorkForEmptyVector) {
  auto in = std::vector<int>{};
  auto expected = in;
  SelectionSort(in);
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, in);
}

TEST(SortTestSimple, SortsWorkForSingleSizeVector) {
  auto in = std::vector<int>{1};
  auto expected = in;
  SelectionSort(in);
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, in);
}

TEST(SortTestSimples, SortsWorkForReverseVector) {
  auto in = std::vector<int>{5, 4, 3, 2, 1};
  auto expected = in;
  SelectionSort(in);
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, in);
}

//-----------------------------------------------------------------------------

class SortTest : public testing::TestWithParam<std::vector<int>> {
  // You can implement all the usual fixture class members here.
  // To access the test parameter, call GetParam() from class
  // TestWithParam<T>.
};

TEST_P(SortTest, WorksForVariousInputs) {
  auto in = GetParam();
  auto expected = in;
  SelectionSort(in);
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, in);
}

INSTANTIATE_TEST_SUITE_P(
    SelectionSortVariousInputs, SortTest,
    testing::Values(std::vector<int>{},             // empty
                    std::vector<int>{1},            // single size
                    std::vector<int>{5, 3, 1, 77},  // small size randome values
                    std::vector<int>{5, 4, 3, 2, 1},  // reversely sorted
                    std::vector<int>{-4, 122, -1000, -4, 122, -1000}
                    // with duplicates
                    ));
