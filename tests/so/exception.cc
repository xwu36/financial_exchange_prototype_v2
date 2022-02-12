// https://stackoverflow.com/questions/70925635/gtest-on-new-keyword

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class base {
 public:
  base(unsigned long size) {
    i = new int[size * size];  // Allocating a high amount of memory.
    std::cout << "base\n";
  }
  int *i;
};

std::string getInstance(base **obj, unsigned long size) {
  base *bObject = new base(size);
  *obj = bObject;  // updated
  return (NULL == bObject)
             ? "NO_MEMORY"
             : "SUCCESS";  // here is problem if new fail it raise an exception.
                           // How to write unit test to catch this?
}

// The Test: Call getdata(), verify sum.
TEST(Test_CUT, TestGet) {
  base *base_obj;

  // Simple usage of EXPECT_THROW. This one should THROW.
  EXPECT_THROW(getInstance(&base_obj, 1000000000000L), std::bad_alloc);

  std::string result1;
  // You can put a block of code in it:
  EXPECT_THROW({ result1 = getInstance(&base_obj, 1000000000000L); },
               std::bad_alloc);
  EXPECT_NE(result1, "SUCCESS");

  std::string result2;
  // This one should NOT throw an exception.
  EXPECT_NO_THROW({ result2 = getInstance(&base_obj, 10L); });
  EXPECT_EQ(result2, "SUCCESS");
}