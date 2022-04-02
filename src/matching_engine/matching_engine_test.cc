#include "src/matching_engine/matching_engine.h"

#include "gtest/gtest.h"
#include "src/order/order.h"
#include "src/order/order_parser.h"

namespace fep::src::matching_engine
{
    namespace
    {
        using ::fep::src::feed_event::FeedEvents;
        using ::fep::src::matching_engine::MatchingEngine;
        using ::fep::src::order::Order;
        using ::nlohmann::json;

        TEST(MatchingEngineTest, ProcessLimitOrders)
        {
            MatchingEngine engine;
            const json data8 = {{"time", 10},
                                {"type", "NEW"},
                                {"order_id", 8},
                                {"symbol", "AAPL"},
                                {"side", "BUY"},
                                {
                                    "quantity",
                                    500,
                                },
                                {"limit_price", "9.90"},
                                {"order_type", "LIMIT"}};
            std::shared_ptr<Order> order8 = std::make_shared<Order>(data8);
            const FeedEvents events1 = engine.Process(order8);
            EXPECT_EQ(events1.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.90\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");

            const json data9 = {{"time", 10},
                                {"type", "NEW"},
                                {"order_id", 9},
                                {"symbol", "AAPL"},
                                {"side", "BUY"},
                                {
                                    "quantity",
                                    500,
                                },
                                {"limit_price", "9.99"},
                                {"order_type", "LIMIT"}};
            const FeedEvents events2 = engine.Process(std::make_shared<Order>(data9.get<Order>()));
            EXPECT_EQ(events2.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");
        }
    }
} // fep::src::matching_engine