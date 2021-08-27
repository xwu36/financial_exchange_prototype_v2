
#include <string>

#include "gtest/gtest.h"

std::string GetHello(std::string_view in) {
  if (in.size() == 0) {
    return std::string("hello, world");
  } else {
    return std::string("Hello, ") + in.data() ;
  }
}

TEST(GetHello, CanMakeHelloWorldWithEmptyInput) {
  EXPECT_EQ(GetHello(""), "hello, world");
}

TEST(GetHello, CanMakeHelloWithNonEmptyInput) {
  EXPECT_EQ(GetHello("Ari"), "Hello, Ari");
}
