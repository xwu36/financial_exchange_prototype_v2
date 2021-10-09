#include <iostream>
#include <string>

#include "src/lib/cpplib.h"

void test() { std::cout << "Test2" << std::endl; }

int main() {
  // Print Hello world!
  // CPPLib s;
  // test();
  // int i = 0;
  // i++;
  // i++;
  // std::cout << s.PrintHelloWorld() << std::endl;

  // char b = 'a';
  char* a = new char;
  *a = 'A';
  std::string s(a);
  std::cout << "s: " << s << std::endl;
  // std::cout << s.fib(6) << std::endl;
  // std::cout << s.fib(4) << std::endl;
  // std::cout << s.fib(8) << std::endl;
  return 0;
}