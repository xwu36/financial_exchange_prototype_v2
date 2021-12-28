
#include <algorithm>  // std::transform
#include <functional>
#include <iterator>  // std::back_inserter
#include <map>
#include <set>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

template <class T>
void PrintVector(std::vector<T> input) {
  std::cout << "{ ";
  unsigned int count = 0;
  for (auto n : input) {
    count++;
    std::cout << n;
    if (count < input.size()) std::cout << ", ";
  }
  std::cout << " }" << std::endl;
}

template <class T>
void Print(const T &input, long infinity = INT_MAX) {
  std::cout << "{ ";
  unsigned int count = 0;
  for (auto n : input) {
    count++;
    if (static_cast<long>(n) == infinity) {
      std::cout << "∞";

    } else {
      std::cout << n;
    }
    if (count < input.size()) std::cout << ", ";
  }
  std::cout << " }" << std::endl;
}

template <class T>
void Print2dVector(std::vector<std::vector<T>> input) {
  std::cout << "{ " << std::endl;
  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < input[0].size(); j++) {
      std::cout << input[i][j];
      if (j < input[0].size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
  }
  std::cout << " }" << std::endl;
}

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
// Method 1: Write separate tests.
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
// Method 2: Write separate tests.
//-----------------------------------------------------------------------------
class SortTestSimpleFixture : public ::testing::Test {
  void TearDown() override {
    expected_ = in_;
    SelectionSort(in_);
    std::sort(expected_.begin(), expected_.end());
    EXPECT_EQ(expected_, in_);
  }

 public:
  std::vector<int> in_;
  std::vector<int> expected_;
};

TEST_F(SortTestSimpleFixture, SortsWorkForEmptyVector) {
  in_ = std::vector<int>{};
}

TEST_F(SortTestSimpleFixture, SortsWorkForSingleSizeVector) {
  in_ = std::vector<int>{1};
}

TEST_F(SortTestSimpleFixture, SortsWorkForReverseVector) {
  in_ = std::vector<int>{5, 4, 3, 2, 1};
}
//-----------------------------------------------------------------------------
// Method 3: Write separate tests.
//-----------------------------------------------------------------------------
TEST(SortTestAllInputs, WorksForAllInputs) {
  std::vector<std::vector<int>> ins = {
      {},                        // empty
      {1},                       // single size
      {5, 3, 1, 77},             // small size random values
      {5, 4, 3, 2, 1},           // reversely sorted
      {-4, 122, -1000, -4, 122}  // with duplicates
  };

  for (auto &in : ins) {
    auto expected = in;
    SelectionSort(in);
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(expected, in);
  }
}

//-----------------------------------------------------------------------------
// Method 3: Parameterized Test.
//-----------------------------------------------------------------------------
class SortTest : public testing::TestWithParam<std::vector<int>> {};

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
                    std::vector<int>{5, 3, 1, 77},  // small size random values
                    std::vector<int>{5, 4, 3, 2, 1},  // reversely sorted
                    std::vector<int>{-4, 122, -1000, -4, 122, -1000}
                    // with duplicates
                    ));

//-----------------------------------------------------------------------------
// Method 3 (Enhanced): Parameterized Test with name generator
//-----------------------------------------------------------------------------
std::map<std::string, std::vector<int>> input_map = {
    //
    {"Empty", {}},
    {"SingleElement", {1}},
    {"ReverseSort", {5, 4, 3, 2, 1}},
    {"SmallVector", {5, 3, 1, 77}},
    {"Duplicates", {-4, 122, -1000, -4, 122, -1000}}
    //
};
class SortTestNameGenerator
    : public testing::TestWithParam<
          std::pair<const std::string, std::vector<int>>> {};

TEST_P(SortTestNameGenerator, WorksForVariousInputs) {
  auto in = GetParam().second;
  auto expected = in;
  SelectionSort(in);
  std::sort(expected.begin(), expected.end());
  EXPECT_EQ(expected, in);
}

INSTANTIATE_TEST_SUITE_P(
    SelectionSortVariousInputsNameGenerator, SortTestNameGenerator,
    testing::ValuesIn(input_map),
    [](const testing::TestParamInfo<SortTestNameGenerator::ParamType> &info) {
      return info.param.first;
    });
