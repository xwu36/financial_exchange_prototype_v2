#include <signal.h>
#include <sys/types.h>

#include <iostream>
#include <string>

#include "gtest/gtest.h"

void MyFunction(int n) {
  EXPECT_EQ(n, 2);
  std::cout << "--------------------------------------" << std::endl;
}

TEST(SubTest, WithoutScopeTraceSimple) {
  MyFunction(1);
  MyFunction(1);
  MyFunction(1);
}

TEST(SubTest, WithoutScopeTraceNestedScopes) {
  MyFunction(1);
  {
    // Scope 1
    MyFunction(1);
    {
      // Scope 2
      MyFunction(1);
    }
  }
}

TEST(SubTest, WithScopeTrace) {
  {
    SCOPED_TRACE("MyScope1");

    MyFunction(1);
    {
      SCOPED_TRACE("MyScope2");
      MyFunction(1);
    }
  }
  MyFunction(1);
}
