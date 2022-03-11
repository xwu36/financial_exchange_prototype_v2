#include <sys/types.h>

#include <csignal>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
void Exit(int exitCode = 0) {
  // Exit with the given code.
  std::exit(exitCode);
}

void Kill(int exitCode = SIGINT) {
  // Kill the process with the given code.s
  kill(getpid(), exitCode);
}

TEST(MyDeathTest, Exit) {
  // Check if Exit() function exits with non-zero code.
  EXPECT_DEATH(Exit(-1), "");
}

TEST(MyDeathTest, NormalExit) {
  EXPECT_EXIT(Exit(), testing::ExitedWithCode(0), "");
}

TEST(MyDeathTest, AbnormalExit) {
  EXPECT_EXIT(Exit(1), testing::ExitedWithCode(1), "");
}

TEST(MyDeathTest, KillProcess) {
  GTEST_FLAG_SET(death_test_style, "threadsafe");
  EXPECT_EXIT(Kill(SIGKILL), testing::KilledBySignal(SIGKILL), "");
}