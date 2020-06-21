
#include "search.h"

#include <algorithm>
#include <cmath>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "search.h"

int Search::BinarySearch(std::vector<int> &nums, int n) {
  return BinarySearchImp(nums, 0, nums.size() - 1, n);
}

int Search::BinarySearchImp(std::vector<int> &nums, int l, int r, int n) {
  if (l > r || nums.size() == 0) {
    return -1;
  }

  int mid = l + (r - l) / 2;
  if (nums[mid] == n) {
    return mid;
  }

  if (nums[mid] > n) {
    return BinarySearchImp(nums, l, mid - 1, n);
  } else {
    return BinarySearchImp(nums, mid + 1, r, n);
  }
};

void Search::BinarySearchRefImp(std::vector<int> &nums, int l, int r, int n,
                                int &result) {
  if (l > r || nums.size() == 0) {
    result = -1;

    return;
  }

  int mid = l + (r - l) / 2;

  if (nums[mid] == n) {
    result = mid;

    return;
  }

  if (nums[mid] > n) {
    BinarySearchRefImp(nums, l, mid - 1, n, result);
  } else {
    BinarySearchRefImp(nums, mid + 1, r, n, result);
  }
};

int Search::BinarySearchPar2(std::vector<int> &nums, int n) {
  if (nums.size() == 0) {
    return -1;
  }

  if (nums.size() == 1) {
    return nums[0] == n ? 0 : -1;
  }

  const int number_of_threads = 1;
  int step = nums.size() / number_of_threads;
  std::vector<std::future<int>> tasks;

  int num_chunks = step == 0 ? 1 : std::ceil((float)nums.size() / (float)step);

  // std::cout << "step: " << step << std::endl;
  // std::cout << "num_chunks: " << num_chunks << std::endl;
  for (int i = 0; i < num_chunks; i++) {
    int low = i * step;
    int high = std::max((i + 1) * step - 1, (int)nums.size() - 1);
    high = std::max(0, high);
    // std::cout << "low: " << low << std::endl;
    // std::cout << "high: " << high << std::endl;
    tasks.push_back(
        std::async(Search::BinarySearchImp, std::ref(nums), low, high, n));
  }

  int cur_offset = 0;
  for (auto &t : tasks) {
    int i = t.get();
    if (i != -1) {
      return i;
    }
    cur_offset += step;
  }

  return -1;
}

int Search::BinarySearchPar(std::vector<int> &nums, int n) {

  const int number_of_threads = 12;
  int step = std::ceil((float)nums.size() / (float)number_of_threads);

  std::vector<int> results(number_of_threads, -1);
  std::vector<std::thread> threads(number_of_threads);

  for (int i = 0; i < number_of_threads; i++) {
    int low = i * step;
    int high = std::min((i + 1) * step - 1, (int)nums.size() - 1);
    high = std::max(0, high);

    threads[i] = std::thread(Search::BinarySearchRefImp, std::ref(nums), low,
                             high, n, std::ref(results[i]));
  }

  for (auto &t : threads) {
    t.join();
  }
  for (auto &r : results) {
    if (r != -1) {
      return r;
    }
  }

  return -1;
}

int Search::TernarySearch(std::vector<int> &nums, int n) {
  return TernarySearchImp(nums, 0, nums.size() - 1, n);
}

int Search::TernarySearchImp(std::vector<int> &nums, int l, int r, int n) {
  if (l > r || nums.size() == 0) {
    return -1;
  }

  // Find the mid1 and mid2
  int mid1 = l + (r - l) / 3;
  int mid2 = r - (r - l) / 3;

  // Check if key is present at any mid
  if (nums[mid1] == n) {
    return mid1;
  }
  if (nums[mid2] == n) {
    return mid2;
  }

  if (n < nums[mid1]) {
    return BinarySearchImp(nums, l, mid1 - 1, n);
  } else if (n > nums[mid2]) {
    return BinarySearchImp(nums, mid2 + 1, r, n);
  } else {
    return BinarySearchImp(nums, mid1 + 1, mid2 - 1, n);
  }
};