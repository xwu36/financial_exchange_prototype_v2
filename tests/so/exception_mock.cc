// https://stackoverflow.com/questions/70925635/gtest-on-new-keyword

#include "gtest/gtest.h"

// Global variable that indicates we are testing. In this case the global new
// operator throws.
bool g_testing = false;

// Overloading Global new operator
void *operator new(size_t sz) {
  void *m = malloc(sz);
  if (g_testing) {
    throw std::bad_alloc();
  }

  return m;
}

class base {
 public:
  base() { std::cout << "base\n"; }
};

std::string getInstance(base **obj) {
  base *bObject = new base();
  *obj = bObject;  // updated
  return (NULL == bObject)
             ? "NO_MEMORY"
             : "SUCCESS";  // here is problem if new fail it raise an exception.
  // How to write unit test to catch this?
}

TEST(Test_New, Failure) {
  base *base_obj;

  // Simple usage of EXPECT_THROW. This one should THROW.
  g_testing = true;
  EXPECT_THROW(getInstance(&base_obj), std::bad_alloc);
  g_testing = false;

  std::string result1;
  // You can put a block of code in it:
  g_testing = true;
  EXPECT_THROW({ result1 = getInstance(&base_obj); }, std::bad_alloc);
  g_testing = false;
  EXPECT_NE(result1, "SUCCESS");
}

TEST(Test_New, Success) {
  base *base_obj;

  std::string result2;
  // This one should NOT throw an exception.
  EXPECT_NO_THROW({ result2 = getInstance(&base_obj); });
  EXPECT_EQ(result2, "SUCCESS");
}