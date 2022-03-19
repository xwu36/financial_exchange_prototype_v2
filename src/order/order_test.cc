#include "src/order/order.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/order/order_parser.h"
#include "nlohmann/json.hpp"

namespace fep::src::order
{
  namespace
  {

    using ::fep::lib::Price4;
    using ::fep::src::stock::Symbol;
    using ::nlohmann::json;
    using ::testing::Field;
    using ::testing::Matcher;

    Matcher<Order> OrderIs(
        const int64_t timestamp_sec,
        const OrderStatus type,
        const int64_t order_id,
        const Symbol symbol,
        const OrderSide side,
        const int32_t quantity,
        const int32_t hidden_quantity,
        const fep::lib::Price4 &price,
        const OrderType order_type,
        const TimeInForce time_in_force)
    {
      return AllOf(
          Field(&Order::timestamp_sec, timestamp_sec),
          Field(&Order::type, type),
          Field(&Order::order_id, order_id),
          Field(&Order::symbol, symbol),
          Field(&Order::side, side),
          Field(&Order::quantity, quantity),
          Field(&Order::hidden_quantity, hidden_quantity),
          Field(&Order::price, price),
          Field(&Order::order_type, order_type),
          Field(&Order::time_in_force, time_in_force));
    }

    TEST(OrderTest, DefaultConstructor)
    {
      Order order{};
      EXPECT_THAT(order, OrderIs(
                             /*timestamp_sec*/ 0,
                             OrderStatus::UNKNOWN,
                             /*order_id*/ 0,
                             Symbol::UNKNOWN,
                             OrderSide::UNKNOWN,
                             /*quantity*/ 0,
                             /*hidden_quantity*/ 0,
                             fep::lib::Price4(0),
                             OrderType::UNKNOWN,
                             TimeInForce::UNKNOWN));
    }

    TEST(OrderUtilsTest, FromJson)
    {
      const json data = {{"time", 123},
                         {"type", "NEW"},
                         {"order_id", 100134},
                         {"symbol", "AAPL"},
                         {"side", "BUY"},
                         {
                             "quantity",
                             100,
                         },
                         {
                             "hidden_quantity",
                             92,
                         },
                         {"limit_price", "140.30"},
                         {"order_type", "MARKET"},
                         {"time_in_force", "IOC"}};
      const Order order = data.get<Order>();
      EXPECT_THAT(order, OrderIs(
                             /*timestamp_sec*/ 123,
                             OrderStatus::NEW,
                             /*order_id*/ 100134,
                             Symbol::AAPL,
                             OrderSide::BUY,
                             /*quantity*/ 100,
                             /*hidden_quantity*/ 99,
                             fep::lib::Price4(1403000),
                             OrderType::MARKET,
                             TimeInForce::IOC));
    }

    TEST(OrderUtilsTest, SideFromJson)
    {
      const json data1 = {{"side", "BUY"}};
      const Order order1 = data1.get<Order>();
      EXPECT_EQ(order1.side, OrderSide::BUY);

      const json data2 = {{"side", "SELL"}};
      const Order order2 = data2.get<Order>();
      EXPECT_EQ(order2.side, OrderSide::SELL);
    }

    TEST(OrderUtilsTest, OrderTypeFromJson)
    {
      const json data1 = {{"order_type", "MARKET"}};
      const Order order1 = data1.get<Order>();
      EXPECT_EQ(order1.order_type, OrderType::MARKET);

      const json data2 = {{"order_type", "LIMIT"}};
      const Order order2 = data2.get<Order>();
      EXPECT_EQ(order2.order_type, OrderType::LIMIT);
    }

    TEST(OrderUtilsTest, OrderStatusFromJson)
    {
      const json data1 = {{"type", "NEW"}};
      const Order order1 = data1.get<Order>();
      EXPECT_EQ(order1.type, OrderStatus::NEW);

      const json data2 = {{"type", "CANCEL"}};
      const Order order2 = data2.get<Order>();
      EXPECT_EQ(order2.type, OrderStatus::CANCEL);
    }

    TEST(OrderUtilsTest, TimeInForce)
    {
      const json data1 = {{"time_in_force", "DAY"}};
      const Order order1 = data1.get<Order>();
      EXPECT_EQ(order1.time_in_force, TimeInForce::DAY);

      const json data2 = {{"time_in_force", "IOC"}};
      const Order order2 = data2.get<Order>();
      EXPECT_EQ(order2.time_in_force, TimeInForce::IOC);

      const json data3 = {{"time_in_force", "GTC"}};
      const Order order3 = data3.get<Order>();
      EXPECT_EQ(order3.time_in_force, TimeInForce::GTC);
    }

  } // namespace
} // namespace fep::src::order