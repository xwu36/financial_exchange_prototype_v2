#include <iostream>
#include <map>
#include <vector>

// Calculates the n'th Fib number.
// Returns a negative value if the input is negative.
int Fib(int n) {
  if (n <= 1) {
    return n;
  }

  return Fib(n - 1) + Fib(n - 2);
}

template <class T>
int ExpectEqual(T expected, T actual) {
  if (expected == actual) {
    return 0;
  } else {
    return 1;
  }
}
int main() {
  int result = 0;
  result += ExpectEqual(Fib(0), 0);
  result += ExpectEqual(Fib(1), 1);
  result += ExpectEqual(Fib(2), 1);
  result += ExpectEqual(Fib(3), 2);

  std::cout << (result == 0 ? "PASS" : "FAIL") << std::endl;
  return 0;
}
