#include "lib/price4.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace fep::lib
{
  namespace
  {

    using ::testing::Field;
    using ::testing::Matcher;

    TEST(Price4Test, FromString)
    {
      const Price4 price1("1234");
      EXPECT_EQ(price1.unscaled(), 12340000);

      const Price4 price2("123.4");
      EXPECT_EQ(price2.unscaled(), 1234000);

      const Price4 price3("12.34");
      EXPECT_EQ(price3.unscaled(), 123400);

      const Price4 price4("1.234");
      EXPECT_EQ(price4.unscaled(), 12340);

      const Price4 price5("0.1234");
      EXPECT_EQ(price5.unscaled(), 1234);

      const Price4 price6("0.01234");
      EXPECT_EQ(price6.unscaled(), 123);

      const Price4 price7("0.001234");
      EXPECT_EQ(price7.unscaled(), 12);
    }

    TEST(Price4Test, ToString)
    {
      const Price4 price1(9900000);
      EXPECT_EQ(price1.to_str(), "990.00");

      const Price4 price2(990000);
      EXPECT_EQ(price2.to_str(), "99.00");

      const Price4 price3(99000);
      EXPECT_EQ(price3.to_str(), "9.90");

      const Price4 price4(9900);
      EXPECT_EQ(price4.to_str(), "0.99");

      const Price4 price5(990);
      EXPECT_EQ(price5.to_str(), "0.099");

      const Price4 price6(99);
      EXPECT_EQ(price6.to_str(), "0.0099");

      const Price4 price7(9.9);
      EXPECT_EQ(price7.to_str(), "0.0009");

      const Price4 price8(0.99);
      EXPECT_EQ(price8.to_str(), "0.00");
    }

    TEST(Price4Test, CompareOperators)
    {
      const Price4 price1(1);
      const Price4 price2(2);
      EXPECT_TRUE(price1 == price1);
      EXPECT_TRUE(price1 != price2);
      EXPECT_TRUE(price1 <= price1);
      EXPECT_TRUE(price1 <= price2);
      EXPECT_TRUE(price1 < price2);
      EXPECT_TRUE(price2 > price1);
      EXPECT_TRUE(price2 >= price1);
      EXPECT_TRUE(price2 >= price2);
    }

    // An example of TEST_F.
    class Price4TestFExampleTest : public testing::Test
    {
      void SetUp() override
      {
        price1_ = Price4(1);
        price2_ = Price4(2);
      }

    protected:
      Price4 price1_;
      Price4 price2_;
    };

    TEST_F(Price4TestFExampleTest, CompareOperators)
    {
      EXPECT_TRUE(price1_ == price1_);
      EXPECT_TRUE(price1_ <= price2_);
    }

    // An example of Matcher
    Matcher<Price4> Price4Is(
        const int a,
        const std::string &b)
    {
      return AllOf(Field(&Price4::a_, a), Field(&Price4::b_, b));
    }

    TEST(Price4MatcherTest, CompareOperators)
    {
      Price4 price1(2);
      price1.a_ = 2;
      price1.b_ = "2";
      const Price4 price2(2);
      EXPECT_THAT(price1, Price4Is(2, "2"));
    }

  } // namespace
} // namespace fep::lib