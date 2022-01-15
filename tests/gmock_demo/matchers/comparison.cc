#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::_;
using ::testing::A;
using ::testing::An;
using ::testing::AnyOf;
using ::testing::ContainerEq;
using ::testing::Contains;
using ::testing::ContainsRegex;
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::FieldsAre;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::HasSubstr;
using ::testing::IsFalse;
using ::testing::IsNull;
using ::testing::IsSubsetOf;
using ::testing::IsTrue;
using ::testing::Le;
using ::testing::Lt;
using ::testing::MatchesRegex;
using ::testing::Ne;
using ::testing::Not;
using ::testing::NotNull;
using ::testing::Pair;
using ::testing::Ref;
using ::testing::StartsWith;
using ::testing::StrCaseEq;
using ::testing::StrCaseNe;
using ::testing::StrEq;
using ::testing::StrNe;
using ::testing::Truly;
using ::testing::TypedEq;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
//-----------------------------------------------------------------------------

TEST(GenericComparison, SimpleComparison) {
  int a = 1;
  EXPECT_THAT(a, Eq(1));
  EXPECT_THAT(a, 1);

  EXPECT_THAT(a, Ge(0));
  EXPECT_THAT(a, Gt(0));
  EXPECT_THAT(a, Lt(2));
  EXPECT_THAT(a, Le(2));
  EXPECT_THAT(a, Ne(2));
}

TEST(GenericComparison, BooleanComparison) {
  int boolean_true = true;
  int boolean_false = false;

  EXPECT_THAT(boolean_true, IsTrue());
  EXPECT_THAT(boolean_false, IsFalse());
}

TEST(GenericComparison, PointerComparison) {
  int* ptr_null = nullptr;
  int* ptr_valid = new int(1);

  EXPECT_THAT(ptr_null, IsNull());
  EXPECT_THAT(ptr_valid, NotNull());
}
//-----------------------------------------------------------------------------

TEST(ReferenceTest, SimpleTest) {
  int a = 1;
  int& a_ref = a;
  int b = 1;
  EXPECT_THAT(a_ref, Ref(a));
  EXPECT_THAT(b, Not(Ref(a)));
}
//-----------------------------------------------------------------------------
TEST(TypedEqTest, SimpleTest) {
  char char_a = 'a';
  int int_a = 'a';
  EXPECT_THAT(char_a, TypedEq<char>('a'));
  EXPECT_THAT(char_a, Not(TypedEq<char>('b')));
  
  EXPECT_THAT(int_a, TypedEq<int>('a'));
  // Doesn't compile:
  // EXPECT_THAT(int_a, TypedEq<char>('a'));
}
//-----------------------------------------------------------------------------
TEST(Wildcard, Underscore) {
  int a = 1;
  std::string str_a = "aa";

  EXPECT_THAT(a, 1);
  EXPECT_THAT(a, _);
  EXPECT_THAT(str_a, _);
}

TEST(Wildcard, AnyType) {
  int a = 1;
  auto b = a;

  EXPECT_THAT(a, A<int>());
  EXPECT_THAT(b, An<int>());
  // Doesn't compile:
  // EXPECT_THAT(a, An<char>());
}