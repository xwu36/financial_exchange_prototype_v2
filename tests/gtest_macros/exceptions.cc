
#include <exception>
#include <iostream>
#include <string>

#include "gtest/gtest.h"

class DivideByZero : public virtual std::exception {
  // what is a virtual function which returns a description
  const char* what() const noexcept override { return "Divide By Zero Error!"; }
};

int Divide(int a, int b) {
  if (b == 0) {
    throw DivideByZero();
  }
  return a / b;
}

TEST(DivideTest, SimpleDivision) {
  EXPECT_EQ(Divide(6, 2), 3);
  EXPECT_EQ(Divide(2, 2), 1);
}

TEST(DivideTest, ThrowsForDivideByZero) {
  EXPECT_THROW(Divide(6, 0), DivideByZero);
  EXPECT_NO_THROW(Divide(6, 2));
}

TEST(DivideTest, DoesNotThrowForDivideByNonZero) {
  EXPECT_NO_THROW(Divide(6, 2));
}