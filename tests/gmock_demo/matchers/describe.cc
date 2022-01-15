/**
 * Note: this test requires C++17
 * Run with:
 * bazel test --cxxopt="-std=c++17" //tests/gmock_demo/matchers:member
 */
#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::_;
using ::testing::Address;
using ::testing::AllOf;
using ::testing::AnyOf;
using ::testing::ContainerEq;
using ::testing::Contains;
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::Field;
using ::testing::FieldsAre;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::HasSubstr;
using ::testing::IsNull;
using ::testing::IsSubsetOf;
using ::testing::Key;
using ::testing::Lt;
using ::testing::Matcher;
using ::testing::Not;
using ::testing::Pair;
using ::testing::Pointee;
using ::testing::Pointer;
using ::testing::Pointwise;
using ::testing::PrintToString;
using ::testing::Property;
using ::testing::Truly;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
using ::testing::WhenDynamicCastTo;
using ::testing::WhenSorted;

// Returns the description of the given matcher.
template <typename T>
std::string Describe(const Matcher<T> &m) {
  return ::testing::DescribeMatcher<T>(m);
}
// Returns the description of the negation of the given matcher.
template <typename T>
std::string DescribeNegation(const Matcher<T> &m) {
  return ::testing::DescribeMatcher<T>(m, true);
}
//-----------------------------------------------------------------------------

TEST(AddressTest, Describe) {
  Matcher<int> matcher = Address(_);
  EXPECT_EQ("has address that is anything", Describe(matcher));
  EXPECT_EQ("does not have address that is anything",
            DescribeNegation(matcher));
}
//-----------------------------------------------------------------------------
TEST(PointerTest, Pointer_RawPointer) {
  int n = 10;
  int *n_ptr1 = &n, *n_ptr2 = &n;
  Matcher<int *> matcher_1 = Pointer(&n);
  Matcher<int *> matcher_2 = AllOf(Pointer(n_ptr1), Pointer(n_ptr2));

  std::ostringstream os1;
  os1 << "is a pointer that is equal to " << n_ptr1;
  EXPECT_EQ(os1.str(), Describe(matcher_1));

  std::ostringstream os2;
  os2 << "("
      << "is a pointer that is equal to " << n_ptr1 << ") "
      << "and "
      << "("
      << "is a pointer that is equal to " << n_ptr2 << ")";
  EXPECT_EQ(os2.str(), Describe(matcher_2));
}
