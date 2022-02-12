#include <map>
#include <vector>
#include <iostream>

// Calculates the n'th Fib number.
// Returns a negative value if the input is negative.
int Fib(int n) {
  if (n <= 1) {
    return n;
  }

  return Fib(n - 1) + Fib(n - 2);
}

int main() {
  int result = Fib(3);

  std::cout << "result: " << result << std::endl;

  result = Fib(5);

  std::cout << "result: " << result << std::endl;
}


// ari
// a
// A
// a234
// 2ab --> invalid
// arash_saif : this is valid in general, only for google test, we don't use _
// arash saif -> invalid
