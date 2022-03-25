#include <iostream>
#include <string>

#include "lib/price4.h"

int main()
{
  // Print Hello World.
  // Test new workspace.
  std::cout << "Hello World!" << std::endl;
  fep::lib::Price4 p("12.34");
  std::cout
      << "unscaled price of " << p.to_str() << " is " << p.unscaled() << std::endl;
  return 0;
}