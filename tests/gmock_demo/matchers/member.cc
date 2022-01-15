/**
 * Note: this test requires C++17
 * Run with:
 * bazel test --cxxopt="-std=c++17" //tests/gmock_demo/matchers:member
 */
#include <algorithm>
#include <iostream>
#include <list>
#include <random>
#include <set>
#include <tuple>
#include <vector>

#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::_;
using ::testing::AnyOf;
using ::testing::ContainerEq;
using ::testing::Contains;
using ::testing::Each;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Eq;
using ::testing::FieldsAre;
using ::testing::Gt;

using ::testing::Field;
using ::testing::Ge;
using ::testing::HasSubstr;
using ::testing::IsSubsetOf;
using ::testing::Key;
using ::testing::Lt;
using ::testing::Matcher;
using ::testing::Not;
using ::testing::Pair;
using ::testing::Pointwise;
using ::testing::PrintToString;
using ::testing::Property;
using ::testing::Truly;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;
using ::testing::WhenSorted;
//-----------------------------------------------------------------------------
TEST(MatchersTest, MemberMatchers) {
  std::map<int, int> m = {{1, 100}, {2, 200}, {3, 300}};

  // Each key is greater than 0 and each value is greater than 99.
  EXPECT_THAT(m, Each(Pair(Gt(0), Gt(99))));

  // Each key is greater than 0.
  EXPECT_THAT(m, Each(Key(Gt(0))));

  // The value is greater than 99.
  EXPECT_THAT(m, Each(Pair(_, Gt(99))));

  // The first part of each pair is greater than 0 and the second part is either
  // greater than 100 or equal to 100.
  EXPECT_THAT(m, Each(FieldsAre(Gt(0), AnyOf(Gt(100), Eq(100)))));
}

TEST(MatchersTest, MemberMatchers_Field_Tuple) {
  std::tuple<int, std::string> my_tuple{7, "hello world"};
  EXPECT_THAT(my_tuple, FieldsAre(Ge(0), HasSubstr("hello")));
}

// Requires C++17
TEST(MatchersTest, MemberMatchers_Field_Struct) {
  struct MyStruct {
    int value = 42;
    std::string greeting = "aloha";
  };
  MyStruct s;
  EXPECT_THAT(s, FieldsAre(42, "aloha"));
}
//-----------------------------------------------------------------------------
// A user-defined class for testing Field.
class MyClass1 {
 public:
  MyClass1(int m) : m_(m) {}

  int m_;
};

TEST(MatchersTest, MemberMatchers_Field_Simple) {
  MyClass1 my_object(1);
  EXPECT_THAT(my_object, Field(&MyClass1::m_, Ge(0)));
  EXPECT_THAT(my_object.m_, Ge(0));
}

TEST(MatchersTest, MemberMatchers_Field_Container) {
  std::vector<MyClass1> my_objects = {MyClass1(1), MyClass1(2), MyClass1(3)};

  EXPECT_THAT(my_objects, Each(Field(&MyClass1::m_, Ge(0))));
  EXPECT_THAT(my_objects, UnorderedElementsAre(Field(&MyClass1::m_, 1),
                                               Field(&MyClass1::m_, 3),
                                               Field(&MyClass1::m_, 2)));
}
//-----------------------------------------------------------------------------
// A user-defined class for testing Property().
class MyClass2 {
 public:
  MyClass2(int m) : m_(m) {}

  int GetM() const { return m_; }
  void SetM(int new_m) { m_ = new_m; }

 private:
  int m_;
};

TEST(MatchersTest, MemberMatchers_Property_Simple) {
  MyClass2 my_object(1);
  EXPECT_THAT(my_object, Property(&MyClass2::GetM, Ge(0)));
  EXPECT_THAT(my_object.GetM(), Ge(0));
}

TEST(MatchersTest, MemberMatchers_Property_Container) {
  std::vector<MyClass2> my_objects = {MyClass2(1), MyClass2(2), MyClass2(3)};

  EXPECT_THAT(my_objects, Each(Property(&MyClass2::GetM, Ge(0))));
  EXPECT_THAT(my_objects, UnorderedElementsAre(Property(&MyClass2::GetM, 1),
                                               Property(&MyClass2::GetM, 3),
                                               Property(&MyClass2::GetM, 2)));
}

//-----------------------------------------------------------------------------
