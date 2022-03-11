
// In this test the the return value is a function of the mock function's
// arguments.
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::WithArg;

class MyMock {
 public:
  MOCK_METHOD(int, method, (int), ());
  MOCK_METHOD(int, method2, (int, int, int, int), ());
};

// Method 1: use WithArg
TEST(MyTest, CanDouble) {
  MyMock mock;

  EXPECT_CALL(mock, method2(_, _, _, _)).WillOnce(WithArg<1>(Invoke([](int x) {
    return x * 2;
  })));

  EXPECT_EQ(mock.method2(1, 5, 1, 1), 10);
}

// Method 2: Use a custom actions by defining a functor:
struct Double {
  template <typename T>
  T operator()(T arg) {
    return 2 * (arg);
  }
};

struct SumOfAll {
  template <typename T>
  T operator()(T arg0, T arg1, T arg2, T arg3) {
    return arg0 + arg1 + arg2 + arg3;
  }
};

TEST(MyTest, CanDoubleWithFunctor) {
  MyMock mock;

  EXPECT_CALL(mock, method(_)).WillRepeatedly(Double{});

  EXPECT_EQ(mock.method(5), 10);
  EXPECT_EQ(mock.method(7), 14);
}

TEST(MyTest, SumOfAllWithFunctor) {
  MyMock mock;

  EXPECT_CALL(mock, method2(_, _, _, _)).WillOnce(SumOfAll{});

  EXPECT_EQ(mock.method2(1, 2, 3, 4), 10);
}