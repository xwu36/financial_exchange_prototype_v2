#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>

#include <iostream>
#include <map>
#include <vector>

/**
 * Shows how to use glog
 */
int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;

  // Removes the prefix from the output
  // FLAGS_log_prefix = false;

  std::vector<int> x = {1, 2, 3, 4};
  std::map<int, int> y = {{1, 2}, {2, 3}};

  LOG(INFO) << "ABC, it's easy as "
            << "{" << x << "}";
  LOG(INFO) << "ABC, it's easy as " << y;

  LOG(INFO) << "This is an info  message";
  LOG(WARNING) << "This is a warning message";
  LOG(INFO) << "Hello, world again!";
  LOG(ERROR) << "This is an error message";
  LOG(FATAL) << "This is a fatal message";
  CHECK(5 == 4) << "Check failed!";

  return 0;
}
