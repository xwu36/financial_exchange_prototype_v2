#include "src/lib/solution.h"
#include <iostream>
#include <vector>

// Example showing misusing pointers
int main() {
  Solution s;
  Person *p = s.ReturnInvalidPointer();

  p->last_name = "Saif";

  std::cout << "p->first_name: " << p->first_name << std::endl;

  return EXIT_SUCCESS;
}