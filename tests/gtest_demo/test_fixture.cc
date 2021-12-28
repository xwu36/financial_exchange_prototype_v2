#include <chrono>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"

// Class we want to test
template <typename E>  // E is the element type.
class Queue {
 private:
  std::vector<E> _v;

 public:
  Queue() {}
  ~Queue() {std::cout << "Queue destructor." << std::endl;}

  void Enqueue(const E& element) { _v.push_back(element); }
  // Throws if the queue is empty.
  E Dequeue() {
    if (IsEmpty()) {
      throw;
    }
    auto tmp = _v[0];

    _v.erase(_v.begin());

    return tmp;
  }
  size_t size() const { return _v.size(); }
  bool IsEmpty() { return _v.empty(); }
};

// Class for test fixture
class QueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    start_time_ = std::chrono::high_resolution_clock::now();
    // GTEST_SKIP() << "Skipping all tests for this fixture";
    q_.Enqueue(1);
    q_.Enqueue(2);
  }

  void TearDown() override {
    auto end_time_ms = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time_ms - start_time_);

    std::cout << "Test took: " << duration.count() << " ms" << std::endl;
    EXPECT_TRUE(duration.count() <= 10000) << "-> The test took too long!";

    PrintQueue();
    std::cout << "=========================Test ended!========================="
              << std::endl;
  }

  void PrintQueue() {
    if (q_.IsEmpty()) {
      std::cout << "-> The queue is empty." << std::endl;
    } else {
      while (!q_.IsEmpty()) {
        std::cout << "-> Remaining item in the queue q_.Dequeue(): "
                  << q_.Dequeue() << std::endl;
      }
    }
  }

  Queue<int> q_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

TEST_F(QueueTest, SizeAndEmptyWork) {
  EXPECT_EQ(q_.size(), 2);
  EXPECT_FALSE(q_.IsEmpty());
}

TEST_F(QueueTest, DequeueWorks) {
  EXPECT_EQ(q_.Dequeue(), 1);
  EXPECT_EQ(q_.Dequeue(), 2);
}

TEST_F(QueueTest, EnqueueWorks) {
  q_.Enqueue(3);
  EXPECT_EQ(q_.Dequeue(), 1);
  EXPECT_EQ(q_.Dequeue(), 2);
  EXPECT_EQ(q_.Dequeue(), 3);
}

TEST_F(QueueTest, TakesTooLong) {
  for (unsigned long i = 0; i < 10000000; i++) {
    q_.Enqueue(1);
    q_.Dequeue();
  }
  EXPECT_EQ(q_.size(), 2);
}