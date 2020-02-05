#ifndef TEMPLATE_SOLUTION_H
#define TEMPLATE_SOLUTION_H

#include <string>
#include <vector>
#include <map>

class Solution {
public:
  std::string PrintHelloWorld();
  int fib(int N);
  private:
  std::map<int, int> _m;
};

#endif