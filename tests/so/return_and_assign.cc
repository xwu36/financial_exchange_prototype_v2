// https://stackoverflow.com/questions/70094725/gmock-expect-call-with-action-to-fill-a-buffer/70764191#70764191
// Return and assign a value to a parameter.
#include <cstring>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::SaveArg;

// The mock class
class MockDataSrc {
 public:
  MOCK_METHOD(int, getdata, (void *, int), ());
};

// The Code-Under-Test:
class CUT {
 public:
  CUT(MockDataSrc *s) { m_s = s; }

  int getandadd() {
    unsigned char buf[32];
    unsigned int sum = 0;
    int n;
    n = m_s->getdata(buf, sizeof(buf));
    for (int t = 0; t < n; t++) {
      sum += buf[t];
    }
    return (int)sum;
  }

 private:
  MockDataSrc *m_s;
};

// The test class:
class Test_CUT : public ::testing::Test {
 public:
  Test_CUT() { source = nullptr; }

 protected:
  void SetUp() override { source = new MockDataSrc(); }

  void TearDown() override {
    delete source;
    source = nullptr;
  }

  MockDataSrc *source;
};

// The Test: Call getdata(), verify sum.
TEST_F(Test_CUT, TestGet) {
  MockDataSrc mock_source;
  CUT cut(&mock_source);
  void *buf;

  static const unsigned char testdata[] = {13, 21, 29, 37};

  // The expect-call here. How can I supply testdata[] in getdata()?
  // Answer: save the buf pointer using SaveArg, then copy into it using the
  // lambda function.
  EXPECT_CALL(mock_source, getdata(::testing::NotNull(), ::testing::Ge(4)))
      .Times(1)
      .WillOnce(DoAll(SaveArg<0>(&buf),
                      Invoke([&buf]() { std::memcpy(buf, testdata, 4); }),
                      Return(4)));

  int sum;
  sum = cut.getandadd();
  EXPECT_EQ(sum, 100);
}