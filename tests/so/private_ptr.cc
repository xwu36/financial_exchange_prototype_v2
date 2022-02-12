// https://stackoverflow.com/questions/70864081/structure-class-to-be-able-to-mock-private-unique-ptr
// Mock private unique pointer

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::Return;

class Bar {
  int f1(int a) { return 2 * a; }
};

class MockBar {
 public:
  MOCK_METHOD(int, f1, (int), ());
};

// The Code-Under-Test:
template <class BAR_TYPE>
class Foo {
 public:
  Foo() : _bar(new BAR_TYPE()) {}
  int DoSomething(int a) { return _bar->f1(a); }

  // A getter was added (Albeit not ideal since it exposes the raw pointer).
  BAR_TYPE *getBar() { return _bar.get(); }

 private:
  std::unique_ptr<BAR_TYPE> _bar;
};

TEST(Test_CUT, TestGet) {
  Foo<MockBar> foo;
  EXPECT_CALL(*(foo.getBar()), f1(2)).WillOnce(Return(4));

  auto result = foo.DoSomething(2);
  EXPECT_EQ(result, 4);
}