#ifndef TEMPLATE_SOLUTION_H
#define TEMPLATE_SOLUTION_H

#include <map>
#include <string>
#include <vector>

/**
 *  Example class used for GTest demo
 */
class Solution {
 public:
  std::string PrintHelloWorld();

  // Calculates the Nth Fibonacci number
  int fib(int N);

 private:
  std::map<int, int> _m;
};

#endif