#include "src/lib/solution.h"
#include <iostream>
#include <map>
#include <vector>

int main() {
  // Print Hello world!
  Solution s;
  std::cout << s.PrintHelloWorld() << std::endl;
  
  // Calculate Fibonacci numbers and puts them in a map
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::map<int, int> results;
  
  for (const auto i : v) {
    int r = s.fib(i);
    results[i] = r;
    std::cout << "r: " << r << std::endl;
  }

  // Prints the results
  for (auto r : results) {
    std::cout << "r.first: " << r.first << ", r.second: " << r.second
              << std::endl;
  }

  return 0;
}