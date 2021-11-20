#include <iostream>
#include <string>

#include "gtest/gtest.h"

class Foo {
 public:
  int PublicMethod(void* x) { return 0; }

 private:
  FRIEND_TEST(FooTest, PrivateMethod);

  int PrivateMethod(void* x) { return x == nullptr ? 0 : 1; }
};

TEST(FooTest, PrivateMethod) {
  Foo foo;
  EXPECT_EQ(foo.PrivateMethod(nullptr), 0);  // Uses Foo's private member Bar().
}