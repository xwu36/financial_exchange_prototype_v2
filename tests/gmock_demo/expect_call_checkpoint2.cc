

#include <exception>
#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

//-----------------------------------------------------------------------------

using ::testing::InSequence;
using ::testing::MockFunction;
class MyMock {
 public:
  MOCK_METHOD(char, Bar, (std::string n), ());
};

MyMock mock;
void Foo(int i) {
  if (i == 1 || i == 3) {
    mock.Bar("a");
  }
}

TEST(FooTest, BarIsCalledTwice) {
  EXPECT_CALL(mock, Bar("a")).Times(2);

  Foo(1);
  Foo(2);
  Foo(3);
}

// The expectation spec says that the first Bar("a") call must happen before
// checkpoint 1, the second Bar("a") call must happen after checkpoint 2,
// and nothing should happen between the two checkpoints.
TEST(FooTest, InvokesBarCorrectly) {
  // Class MockFunction<F> has exactly one mock method.  It is named
  // Call() and has type F.
  MockFunction<void(std::string check_point_name)> check;
  {
    InSequence s;

    EXPECT_CALL(mock, Bar("a"));
    EXPECT_CALL(check, Call("1"));
    EXPECT_CALL(check, Call("2"));
    EXPECT_CALL(mock, Bar("a"));
  }

  // Bar should be called once
  Foo(1);
  check.Call("1");

  // Bar should not be called
  Foo(4);
  Foo(2);
  Foo(5);

  // Bar should be called once
  check.Call("2");
  Foo(3);
}