#ifndef SORT_H
#define SORT_H

#include <algorithm>
#include <vector>

class Search {
 public:
  static int BinarySearch(std::vector<int>& nums, int n);
  static int TernarySearch(std::vector<int>& nums, int n);

  static int BinarySearchPar(std::vector<int>& nums, int n);

  static int BinarySearchImp(std::vector<int>& nums, int l, int r, int n);
  static int TernarySearchImp(std::vector<int>& nums, int l, int r, int n);

  static void BinarySearchRefImp(std::vector<int>& nums, int l, int r, int n,
                                 int& result);
  static int BinarySearchPar2(std::vector<int>& nums, int n);
};

#endif