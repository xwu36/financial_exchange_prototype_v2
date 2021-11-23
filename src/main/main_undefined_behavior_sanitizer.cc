// Demo of undefined behavior Sanitizer.
// By Ari Saif
// Run with this command:
// bazel run --config=ubsan //src/main:main_undefined_behavior_sanitizer \\
// -- --choice=[choice]
// , where choice is one of the values from 0 to 6. See the main function below.

#include <iostream>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"

ABSL_FLAG(uint32_t, choice, 0, "choice");
//-----------------------------------------------------------------------------
int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  int choice = absl::GetFlag(FLAGS_choice);

  switch (choice) {
    case 0: {
      int k = 0x7fffffff;
      k += 100;  // undefined behavior
      std::cout << "k: " << k << std::endl;
      break;
    }
    case 1: {
      int x = 1;
      return x / (x - 1);  // undefined behavior
    }

    case 2: {
      int x = 10;
      int arr[4] = {0, 1, 2, 3};
      int a = arr[x];  // undefined behavior for indexing out of bounds
      std::cout << "a: " << a << std::endl;
      break;
    }

    case 3: {
      int *p = 0;
      int a = *p;  // undefined behavior for dereferencing a null pointer
      std::cout << "a: " << a << std::endl;
      break;
    }

    case 4: {
      int num = -1;
      unsigned int val =
          1 << num;  // shifting by a negative number - undefined behavior
      std::cout << "val: " << val << std::endl;
      break;
    }

    case 5: {
      int num = 32;  // or whatever number greater than 31
      int val =
          1 << num;  // the literal '1' is typed as a 32-bit integer - in this
                     // case shifting by more than 31 bits is undefined behavior

      std::cout << "val: " << val << std::endl;
      break;
    }

    // Can't be detected by udef sanitizer :(
    case 20: {
      int a = 0;
      int b = 0;
      return &a < &b;  // undefined behavior
    }

    // Can't be detected by udef sanitizer :(
    case 21: {
      int arr[4] = {0, 1, 2, 3};
      int i = 0;
      arr[i] = i++;  // undefined behavior
    }

    default:
      std::cout << "Error: Invalid choice value: " << choice << std::endl;
  }
}