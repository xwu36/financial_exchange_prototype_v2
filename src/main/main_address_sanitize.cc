// Demo of Address Sanitizer.
// By Ari Saif
// Run with this command:
// bazel run --config=asan //src/main:main_address_sanitize -- --choice=[choice]
// where choice is one of the values from 0 to 6. See the main function below.

#include <iostream>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"

ABSL_FLAG(uint32_t, choice, 0, "choice");
//-----------------------------------------------------------------------------
int *ptr;
__attribute__((noinline)) void FunctionThatEscapesLocalObject() {
  int local[100];
  ptr = &local[0];
}

int global_array[100] = {-1};
volatile int *volatile_ptr = 0;
//-----------------------------------------------------------------------------
int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  int choice = absl::GetFlag(FLAGS_choice);

  switch (choice) {
    case 0: {
      // Usage after delete.
      int *p = new int;
      delete p;
      std::cout << "*p: " << *p << std::endl;
      break;
    }

    case 1: {
      // SEGV on unknown address
      std::vector<int> v;
      std::cout << "v[100]: " << v[100] << std::endl;
      break;
    }
    case 2: {
      // Heap buffer overflow.
      int *array = new int[100];
      array[0] = 0;
      int res = array[10 + 100];  // BOOM
      delete[] array;
      break;
    }

    case 3: {
      // Leak detection (Doesn't work on Mac?)
      // Set this env variable before running:
      // export ASAN_OPTIONS=detect_leaks=1
      int *p = new int;
      *p = 10;
      std::cout << "*p: " << *p << std::endl;
      break;
    }

    case 4: {
      // stack-use-after-return
      // Set this env variable before running:
      // export ASAN_OPTIONS=detect_stack_use_after_return=1
      FunctionThatEscapesLocalObject();
      return ptr[argc];
    }

    case 5: {
      // global-buffer-overflow
      return global_array[argc + 100];  // BOOM
    }

    case 6: {
      // stack-use-after-scope
      {
        int x = 0;
        volatile_ptr = &x;
      }
      *volatile_ptr = 5;
    }
    default:
      std::cout << "Error: Invalid choice value: " << choice << std::endl;
  }

  return 0;
}