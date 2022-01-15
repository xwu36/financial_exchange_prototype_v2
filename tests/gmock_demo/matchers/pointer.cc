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

//-----------------------------------------------------------------------------
void Test() {
  // Pointee of a_ptr is a;
  // Pointee of a_ptr is a_ptr;
  // Address of a is in a_ptr.
  int a = 1;
  int *a_ptr = &a;

  // Pointee of u_ptr is 10.
  // pointer of u_ptr is u_ptr.get().
  // Address of 10 is in u_ptr.
  std::unique_ptr<int> u_ptr(new int(10));
  int *pointer = u_ptr.get();
}

TEST(PointerTest, Pointee_RawPointer_Simple) {
  int *n_ptr = new int(10);
  EXPECT_THAT(*n_ptr, Ge(9));
  EXPECT_THAT(n_ptr, Pointee(Ge(9)));
}

TEST(PointerTest, Pointee_RawPointer_Container) {
  int a = 1, b = 2;
  std::vector<int *> my_pointers = {&a, &b};
  EXPECT_THAT(my_pointers, Each(Pointee(Ge(0))));
  EXPECT_THAT(my_pointers, UnorderedElementsAre(Pointee(b), Pointee(a)));

  // Doesn't work!
  // EXPECT_THAT(my_pointers, UnorderedElementsAre(b, a));
}

TEST(PointerTest, Pointee_SmartPointer_Simple) {
  std::unique_ptr<int> n_ptr(new int(10));
  EXPECT_THAT(*n_ptr, Ge(9));
  EXPECT_THAT(n_ptr, Pointee(Ge(0)));
}

TEST(PointerTest, Pointee_SmartPointer_Container) {
  std::vector<std::unique_ptr<int>> my_pointers;
  my_pointers.push_back(std::unique_ptr<int>(new int(1)));
  my_pointers.push_back(std::unique_ptr<int>(new int(2)));

  EXPECT_THAT(my_pointers, Each(Pointee(Ge(0))));
  EXPECT_THAT(my_pointers, UnorderedElementsAre(Pointee(2), Pointee(1)));
}
//-----------------------------------------------------------------------------
TEST(PointerTest, Address) {
  int n = 10;
  int *n_ptr1 = &n, *n_ptr2 = &n;

  EXPECT_THAT(n, Address(n_ptr1));
  EXPECT_THAT(n, Address(AllOf(n_ptr1, n_ptr2)));
}

TEST(PointerTest, Address_Container2) {
  std::vector<int> my_objects = {1, 2};

  EXPECT_THAT(my_objects, UnorderedElementsAre(Address(&my_objects[0] + 1),
                                               Address(&my_objects[0])));
}

//-----------------------------------------------------------------------------
TEST(PointerTest, Pointer_RawPointer) {
  int n = 10;
  int *n_ptr1 = &n, *n_ptr2 = &n;
  EXPECT_THAT(&n, Pointer(&n));
  EXPECT_THAT(n_ptr1, Pointer(&n));
  EXPECT_THAT(&n, AllOf(Pointer(n_ptr1), Pointer(n_ptr2)));
}
TEST(PointerTest, Pointer_SmartPointer_Simple) {
  std::unique_ptr<int> n(new int(10));
  EXPECT_THAT(n, Pointer(n.get()));
}
TEST(PointerTest, Pointer_SmartPointer_Simple2) {
  std::unique_ptr<int> a(new int(1));
  std::unique_ptr<int> b(new int(1));

  EXPECT_THAT(a, Pointer(a.get()));
  EXPECT_THAT(a, Not(Pointer(b.get())));
}

TEST(PointerTest, Pointer_SmartPointer_Container) {
  std::unique_ptr<int> n(new int(10));

  EXPECT_THAT(n, Pointer(n.get()));

  std::vector<std::unique_ptr<int>> my_pointers;
  my_pointers.push_back(std::unique_ptr<int>(new int(1)));
  my_pointers.push_back(std::unique_ptr<int>(new int(2)));

  EXPECT_THAT(my_pointers, UnorderedElementsAre(Pointer(my_pointers[1].get()),
                                                Pointer(my_pointers[0].get())));

  // Doesn't work:
  // EXPECT_THAT(my_pointers,
  //             UnorderedElementsAre((my_pointers[1]), (my_pointers[0])));
  // EXPECT_THAT(my_pointers,
  //             UnorderedElementsAreArray({(my_pointers[1]),
  //             (my_pointers[0])}));

  // EXPECT_THAT(my_pointers, UnorderedElementsAre((my_pointers[1].get()),
  //                                               (my_pointers[0].get())));
}

TEST(PointerTest, Pointer_SharedPointer) {
  auto a_ptr = std::make_shared<int>(1);
  auto b_ptr = a_ptr;

  std::vector<std::shared_ptr<int>> my_pointers = {a_ptr, b_ptr};

  EXPECT_THAT(my_pointers, UnorderedElementsAre(a_ptr, b_ptr));
}

//-----------------------------------------------------------------------------
class Base {
 public:
  Base() { std::cout << "BASE" << std::endl; }
  virtual ~Base() {}
};

class Derived : public Base {
 public:
  Derived() { std::cout << "DERIVED" << std::endl; }
  int i;
};

TEST(WhenDynamicCastToTest, SimpleTest) {
  Derived derived;
  derived.i = 4;

  Base *base_ptr_derived = &derived;
  Base *base_ptr = new Base();

  EXPECT_THAT(dynamic_cast<Derived *>(base_ptr_derived), Not(IsNull()));
  EXPECT_THAT(dynamic_cast<Derived *>(base_ptr_derived), Field(&Derived::i, 4));
  EXPECT_THAT(dynamic_cast<Derived *>(base_ptr), IsNull());

  EXPECT_THAT(base_ptr_derived, WhenDynamicCastTo<Derived *>(Not(IsNull())));
  EXPECT_THAT(base_ptr_derived,
              WhenDynamicCastTo<Derived *>(Field(&Derived::i, 4)));
}