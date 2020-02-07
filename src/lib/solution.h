#ifndef TEMPLATE_SOLUTION_H
#define TEMPLATE_SOLUTION_H

#include <map>
#include <string>
#include <vector>

class Solution {
public:
  std::string PrintHelloWorld();
  int fib(int N);

private:
  std::map<int, int> _m;
};

#endif