#include "solution.h"

std::string Solution::PrintHelloWorld() { 
  return "**** Hello World ****"; 
}

// Calculates the Nth Fibonacci number
int Solution::fib(int N) {
    if (N == 0) {
      return 0;
    }

    if (N == 1) {
      return 1;
    }

    if (_m.count(N)) {
      return _m[N];
    } else {
      _m[N] = fib(N - 1) + fib(N - 2);
      return _m[N];
    }
  }


