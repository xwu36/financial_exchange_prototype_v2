
#include <iostream>
#include <string>

#include "gtest/gtest.h"
//-----------------------------------------------------------------------------
const char kString1[] = "String 1";
const char kString11[] = "STRING 1";
const char kString2[] = "String 2";
//-----------------------------------------------------------------------------
TEST(CStringTest, SimpleComparisons) {
  EXPECT_STREQ(kString1, "String 1");
  EXPECT_STRNE(kString1, kString2);

  EXPECT_STRNE(kString1, kString11);
  EXPECT_STRCASEEQ(kString1, kString1);
}
