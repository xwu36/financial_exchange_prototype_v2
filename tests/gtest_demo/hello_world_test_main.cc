
#include <string>

#include "gtest/gtest.h"

std::string GetHello(std::string_view in) {
  if (in.size() == 0) {
    return std::string("hello, world");
  } else {
    return std::string("Hello, ") + in.data();
  }
}

TEST(GetHello, CanMakeHelloWorldWithEmptyInput) {
  EXPECT_EQ(GetHello(""), "hello, world");
}

TEST(GetHello, CanMakeHelloWithNonEmptyInput) {
  EXPECT_EQ(GetHello("Ari"), "Hello, Ari");
}

namespace testing {
namespace internal {
extern bool g_help_flag;
}
}  // namespace testing

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  std::cout << "::testing::GTEST_FLAG(list_tests): "
            << ::testing::GTEST_FLAG(list_tests) << std::endl;

             
  std::cout << "g_help_flag: " << ::testing::internal::g_help_flag << std::endl;

  std::cout << "::testing::GTEST_FLAG(repeat): " << ::testing::GTEST_FLAG(repeat) << std::endl;
  return RUN_ALL_TESTS();
}