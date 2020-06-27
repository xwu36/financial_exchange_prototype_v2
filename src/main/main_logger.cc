/**
 * Example main file for C++ starter template
 * showcasing the usage of GLog library 
 * By: Ari Saif
 */
#include "glog/logging.h"
#include "glog/stl_logging.h"

#include <iostream>
#include <map>
#include <vector>
bool g_cond = true;

void MyFunction() {
  // Removes the prefix from the output
  // FLAGS_log_prefix = false;
  std::vector<int> my_vector = {1, 2, 3, 4};
  std::map<int, int> my_map = {{1, 2}, {2, 3}};

  LOG(INFO) << "Printing my_vector: "
            << "{" << my_vector << "}";

  LOG(INFO) << "Printing a my_map " << my_map;
  LOG(WARNING) << "This is a warning message";
  LOG(INFO) << "Hello, world again!";
  LOG(ERROR) << "This is an error message";
  LOG_IF(INFO, g_cond == true) << "g_cond is true!";
  CHECK(5 == 4) << "Check failed!";
}

/**
 * Shows how to use GLOg
 */
int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;
  MyFunction();
  return 0;
}
