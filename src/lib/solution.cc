#include "solution.h"

std::string Solution::PrintHelloWorld() { return "**** Hello World ****"; }

Person *Solution::ReturnInvalidPointer() {
  Person *p = nullptr;
  p->first_name = "Ari";

  return p;
};

// Calculates the Nth Fibonacci number
int Solution::fib(int N) {
  if (N == 0) {
    return 0;
  }

  if (N == 1) {
    return 1;
  }

  return fib(N - 1) + fib(N - 2);
}
