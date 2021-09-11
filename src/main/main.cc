#include <iostream>

#include "src/lib/cpplib.h"

void test(){
  std::cout << "Test2" << std::endl;
}

int main() {
  // Print Hello world!
  CPPLib s;
  test();
  std::cout << s.PrintHelloWorld() << std::endl;

  // std::cout << s.fib(6) << std::endl;
  // std::cout << s.fib(4) << std::endl;
  // std::cout << s.fib(8) << std::endl;
  return 0;
}