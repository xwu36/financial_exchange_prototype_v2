#include "src/lib/solution.h"
#include <iostream>

int main() {
  Solution s;
  std::cout << s.PrintHelloWorld() << std::endl;

  int r = s.fib(10);
  int j = s.fib(12);
  std::cout << "r: " << r << std::endl;
  return EXIT_SUCCESS;
}