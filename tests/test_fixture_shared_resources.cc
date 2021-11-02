#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

class FooTest : public testing::Test {
 protected:
  // Per-test-suite set-up.
  // Called before the first test in this test suite.
  // Can be omitted if not needed.
  static void SetUpTestSuite() {
    std::cout << "========Beginning of all tests========" << std::endl;
    // Avoid reallocating static objects if called in subclasses of FooTest.
    if (shared_resource_ == nullptr) {
      shared_resource_ =
          new std::string("This is an expensive shared resource!");
    }
  }

  // Per-test-suite tear-down.
  // Called after the last test in this test suite.
  // Can be omitted if not needed.
  static void TearDownTestSuite() {
    std::cout << "========End of all tests========" << std::endl;
    delete shared_resource_;
    shared_resource_ = nullptr;
  }

  // You can define per-test set-up logic as usual.
  void SetUp() override {
    std::cout << "Called before each test." << std::endl;
  }

  // You can define per-test tear-down logic as usual.
  void TearDown() override {
    std::cout << "Called after each test." << std::endl;
  }

  // Some expensive resource shared by all tests.
  static std::string* shared_resource_;
};

std::string* FooTest::shared_resource_ = nullptr;

TEST_F(FooTest, Test1) { EXPECT_EQ(1, 1); }

TEST_F(FooTest, Test2) { EXPECT_EQ(2, 2); }