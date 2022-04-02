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

            const json data5 = {{"time", 10},
                                {"type", "NEW"},
                                {"order_id", 5},
                                {"symbol", "AAPL"},
                                {"side", "BUY"},
                                {
                                    "quantity",
                                    500,
                                },
                                {"limit_price", "10.00"},
                                {"order_type", "LIMIT"}};
            const FeedEvents events3 = engine.Process(std::make_shared<Order>(data5.get<Order>()));
            EXPECT_EQ(events3.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.00\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");

            const json data10 = {{"time", 10},
                                 {"type", "NEW"},
                                 {"order_id", 10},
                                 {"symbol", "AAPL"},
                                 {"side", "BUY"},
                                 {
                                     "quantity",
                                     200,
                                 },
                                 {"limit_price", "10.01"},
                                 {"order_type", "LIMIT"}};
            const FeedEvents events4 = engine.Process(std::make_shared<Order>(data10.get<Order>()));
            EXPECT_EQ(events4.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":200}\"]},{\"ask\":null}]");

            const json data1 = {{"time", 9},
                                {"type", "NEW"},
                                {"order_id", 1},
                                {"symbol", "AAPL"},
                                {"side", "BUY"},
                                {
                                    "quantity",
                                    100,
                                },
                                {"limit_price", "10.01"},
                                {"order_type", "LIMIT"}};
            const FeedEvents events5 = engine.Process(std::make_shared<Order>(data1.get<Order>()));
            EXPECT_EQ(events5.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":300}\"]},{\"ask\":null}]");

            const json data12 = {{"time", 10},
                                 {"type", "NEW"},
                                 {"order_id", 12},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     100,
                                 },
                                 {"limit_price", "10.02"},
                                 {"order_type", "LIMIT"}};
            const FeedEvents events6 = engine.Process(std::make_shared<Order>(data12.get<Order>()));
            EXPECT_EQ(events6.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.02\\\",\\\"quantity\\\":100}\"]}]");

            const json data2 = {{"time", 9},
                                {"type", "NEW"},
                                {"order_id", 2},
                                {"symbol", "AAPL"},
                                {"side", "SELL"},
                                {
                                    "quantity",
                                    500,
                                },
                                {"limit_price", "10.03"},
                                {"order_type", "LIMIT"}};
            const FeedEvents events7 = engine.Process(std::make_shared<Order>(data2.get<Order>()));
            EXPECT_EQ(events7.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":500}\"]}]");

            const json data3 = {{"time", 10},
                                {"type", "NEW"},
                                {"order_id", 3},
                                {"symbol", "AAPL"},
                                {"side", "SELL"},
                                {
                                    "quantity",
                                    200,
                                },
                                {"limit_price", "10.03"},
                                {"order_type", "LIMIT"}};
            const FeedEvents events8 = engine.Process(std::make_shared<Order>(data3.get<Order>()));
            EXPECT_EQ(events8.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":700}\"]}]");

            const json data14 = {{"time", 9},
                                 {"type", "NEW"},
                                 {"order_id", 14},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     1000,
                                 },
                                 {"limit_price", "10.04"},
                                 {"order_type", "LIMIT"}};
            const FeedEvents events9 = engine.Process(std::make_shared<Order>(data14.get<Order>()));
            EXPECT_EQ(events9.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.04\\\",\\\"quantity\\\":1000}\"]}]");

            // Case 1 with Order15.
            const json data15 = {{"time", 20},
                                 {"type", "NEW"},
                                 {"order_id", 15},
                                 {"symbol", "AAPL"},
                                 {"side", "BUY"},
                                 {
                                     "quantity",
                                     200,
                                 },
                                 {"limit_price", "10.03"},
                                 {"order_type", "LIMIT"}};
            const FeedEvents events10 = engine.Process(std::make_shared<Order>(data15.get<Order>()));
            EXPECT_EQ(events10.to_str(), "{\"price\":\"10.02\",\"quantity\":100,\"type\":\"TRADE\"}/n{\"price\":\"10.03\",\"quantity\":100,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":600}\",\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"10.02\\\",\\\"quantity\\\":0}\"]}]");

            // Case 2 with Order16.
            const json data16 = {{"time", 21},
                                 {"type", "NEW"},
                                 {"order_id", 16},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     600,
                                 },
                                 {"limit_price", "10.02"},
                                 {"order_type", "LIMIT"}};
            const FeedEvents events11 = engine.Process(std::make_shared<Order>(data16.get<Order>()));
            EXPECT_EQ(events11.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.02\\\",\\\"quantity\\\":600}\"]}]");

            // Case 3 with Order17.
            const json data17 = {{"time", 22},
                                 {"type", "NEW"},
                                 {"order_id", 17},
                                 {"symbol", "AAPL"},
                                 {"side", "SELL"},
                                 {
                                     "quantity",
                                     1000,
                                 },
                                 {"limit_price", "10.00"},
                                 {"order_type", "LIMIT"}};
            const FeedEvents events12 = engine.Process(std::make_shared<Order>(data17.get<Order>()));
            EXPECT_EQ(events12.to_str(), "{\"price\":\"10.01\",\"quantity\":200,\"type\":\"TRADE\"}/n{\"price\":\"10.01\",\"quantity\":100,\"type\":\"TRADE\"}/n{\"price\":\"10.00\",\"quantity\":500,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"10.00\\\",\\\"quantity\\\":0}\",\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"10.01\\\",\\\"quantity\\\":0}\"]},{\"ask\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"10.00\\\",\\\"quantity\\\":200}\"]}]");

            // Case 4 with Order18.
            const json data18 = {{"time", 23},
                                 {"type", "CANCEL"},
                                 {"order_id", 2}};
            const FeedEvents events13 = engine.Process(std::make_shared<Order>(data18.get<Order>()));
            EXPECT_EQ(events13.to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"10.03\\\",\\\"quantity\\\":200}\"]}]");

            // TEST_F(MatchingEngineImplTest, ProcessMarketOrders)
            // {
            //     MatchingEngineImpl engine(rule_, /*lot_size=*/10);
            //     const json data8 = {{"time", 10},
            //                         {"type", "NEW"},
            //                         {"order_id", 8},
            //                         {"symbol", "AAPL"},
            //                         {"side", "BUY"},
            //                         {
            //                             "quantity",
            //                             500,
            //                         },
            //                         {"order_type", "MARKET"}};
            //     std::unique_ptr<Order> order8 = std::make_unique<Order>(data8);
            //     const auto &status1 = engine.Process(std::move(order8));
            //     ASSERT_TRUE(status1.ok());
            //     EXPECT_EQ(status1->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":null}]");

            //     const json data9 = {{"time", 10},
            //                         {"type", "NEW"},
            //                         {"order_id", 9},
            //                         {"symbol", "AAPL"},
            //                         {"side", "BUY"},
            //                         {
            //                             "quantity",
            //                             500,
            //                         },
            //                         {"limit_price", "9.99"},
            //                         {"order_type", "LIMIT"}};
            //     std::unique_ptr<Order> order9 = std::make_unique<Order>(data9);
            //     const auto &status2 = engine.Process(std::move(order9));
            //     ASSERT_TRUE(status2.ok());
            //     EXPECT_EQ(status2->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":500}\"]},{\"ask\":null}]");

            //     const json data5 = {{"time", 10},
            //                         {"type", "NEW"},
            //                         {"order_id", 5},
            //                         {"symbol", "AAPL"},
            //                         {"side", "SELL"},
            //                         {
            //                             "quantity",
            //                             200,
            //                         },
            //                         {"order_type", "MARKET"}};
            //     std::unique_ptr<Order> order5 = std::make_unique<Order>(data5);
            //     const auto &status3 = engine.Process(std::move(order5));
            //     ASSERT_TRUE(status3.ok());
            //     EXPECT_EQ(status3->to_str(), "{\"price\":\"9.99\",\"quantity\":200,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":300}\"]},{\"ask\":null}]");

            //     const json data10 = {{"time", 10},
            //                          {"type", "NEW"},
            //                          {"order_id", 10},
            //                          {"symbol", "AAPL"},
            //                          {"side", "SELL"},
            //                          {
            //                              "quantity",
            //                              1000,
            //                          },
            //                          {"order_type", "MARKET"}};
            //     std::unique_ptr<Order> order10 = std::make_unique<Order>(data10);
            //     const auto &status4 = engine.Process(std::move(order10));
            //     ASSERT_TRUE(status4.ok());
            //     EXPECT_EQ(status4->to_str(), "{\"price\":\"9.99\",\"quantity\":300,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"9.99\\\",\\\"quantity\\\":0}\"]},{\"ask\":null}]");

            //     const json data1 = {{"time", 9},
            //                         {"type", "NEW"},
            //                         {"order_id", 1},
            //                         {"symbol", "AAPL"},
            //                         {"side", "BUY"},
            //                         {
            //                             "quantity",
            //                             100,
            //                         },
            //                         {"order_type", "MARKET"}};
            //     std::unique_ptr<Order> order1 = std::make_unique<Order>(data1);
            //     const auto &status5 = engine.Process(std::move(order1));
            //     ASSERT_TRUE(status5.ok());
            //     EXPECT_EQ(status5->to_str(), "[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":null},{\"ask\":null}]");
        }
    }
} // fep::src::matching_engine