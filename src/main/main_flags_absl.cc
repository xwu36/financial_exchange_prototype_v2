#include <iostream>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"

ABSL_FLAG(bool, verbose, false,
          "Include 'advanced' options in the menu listing");
ABSL_FLAG(std::string, message, "Hello world!", "Message to print");
ABSL_FLAG(uint32_t, year, 2020, "Current year");

int main(int argc, char *argv[]) {
  absl::SetProgramUsageMessage("--massage: custom message");
  absl::ParseCommandLine(argc, argv);

  if (absl::GetFlag(FLAGS_verbose)) {
    std::cout << "Verbose "
              << ": ";
  }
  std::cout << absl::GetFlag(FLAGS_message) << std::endl;
  return 0;
}