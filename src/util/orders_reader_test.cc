#include "src/util/orders_reader.h"

#include "lib/price4.h"
#include "gtest/gtest.h"

namespace fep::src::util
{
    namespace
    {

        using ::fep::lib::Price4;
        using ::fep::src::order::Order;
        using ::fep::src::order::OrderSide;
        using ::fep::src::order::OrderStatus;
        using ::fep::src::order::OrderType;
        using ::fep::src::stock::Symbol;

        TEST(OrderUtilsTest, ReadOrdersFromPath)
        {
            const std::vector<Order> orders = ReadOrdersFromPath("src/util/test_data/orders.jsonl");
            EXPECT_EQ(orders.size(), 2);
            EXPECT_EQ(orders[0].order_id, 100134);
            EXPECT_EQ(orders[0].order_type, OrderType::LIMIT);
            EXPECT_EQ(orders[0].price, Price4(1403000));
            EXPECT_EQ(orders[0].quantity, 100);
            EXPECT_EQ(orders[0].side, OrderSide::BUY);
            EXPECT_EQ(orders[0].symbol, Symbol::AAPL);
            EXPECT_EQ(orders[0].timestamp_sec, 1625787615);
            EXPECT_EQ(orders[0].type, OrderStatus::NEW);

            EXPECT_EQ(orders[1].order_id, 100135);
            EXPECT_EQ(orders[1].order_type, OrderType::MARKET);
            EXPECT_EQ(orders[1].price, Price4(1423000));
            EXPECT_EQ(orders[1].quantity, 101);
            EXPECT_EQ(orders[1].side, OrderSide::SELL);
            EXPECT_EQ(orders[1].symbol, Symbol::TSLA);
            EXPECT_EQ(orders[1].timestamp_sec, 1725787615);
            EXPECT_EQ(orders[1].type, OrderStatus::CANCEL);
        }

    } // namespace
} // namespace fep::src::util