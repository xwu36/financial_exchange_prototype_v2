#include "src/feed_event/feed_event.h"

#include <memory>

#include "gtest/gtest.h"

namespace fep::src::feed_event
{
    namespace
    {
        using fep::lib::Price4;

        TEST(TradeMessageTest, TradeResultToString)
        {
            const OrderTradeEvent result{
                .price = Price4("12.32"),
                .quantity = 200};
            EXPECT_EQ(result.to_str(), std::string("{\"price\":\"12.32\",\"quantity\":200,\"type\":\"TRADE\"}"));
        }

        TEST(FeedEventTest, TradeUpdate)
        {
            DepthUpdateEvents events;
            std::shared_ptr<PriceEntityUpdateEvent> update_event1 = std::make_shared<PriceEntityAddEvent>(Price4("12.32"), 200);
            std::shared_ptr<PriceEntityUpdateEvent> update_event2 = std::make_shared<PriceEntityAddEvent>(Price4("11.32"), 100);
            std::shared_ptr<PriceEntityUpdateEvent> update_event3 = std::make_shared<PriceEntityDeleteEvent>(Price4("13.32"), 300);
            std::shared_ptr<PriceEntityUpdateEvent> update_event4 = std::make_shared<PriceEntityModifyEvent>(Price4("14.32"), 400);
            events.bid_events.push_back(update_event1);
            events.bid_events.push_back(update_event2);
            events.ask_events.push_back(update_event3);
            events.ask_events.push_back(update_event4);
            EXPECT_EQ(events.to_str(), std::string("[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"12.32\\\",\\\"quantity\\\":200}\",\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"11.32\\\",\\\"quantity\\\":100}\"]},{\"ask\":[\"{\\\"action\\\":\\\"DELETE\\\",\\\"price\\\":\\\"13.32\\\",\\\"quantity\\\":300}\",\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"14.32\\\",\\\"quantity\\\":400}\"]}]"));
        }

        TEST(FeedEventTest, TradeMessage)
        {
            FeedEvents events;
            events.order_trade_events.push_back(OrderTradeEvent{
                .price = Price4("12.32"),
                .quantity = 200});
            events.order_trade_events.push_back(OrderTradeEvent{
                .price = Price4("11.32"),
                .quantity = 100});
            std::shared_ptr<PriceEntityUpdateEvent> update_event1 = std::make_shared<PriceEntityAddEvent>(Price4("11.32"), 100);
            std::shared_ptr<PriceEntityUpdateEvent> update_event2 = std::make_shared<PriceEntityModifyEvent>(Price4("14.32"), 400);
            events.depth_update_events.bid_events.push_back(update_event1);
            events.depth_update_events.bid_events.push_back(update_event2);
            EXPECT_EQ(events.to_str(), std::string("{\"price\":\"12.32\",\"quantity\":200,\"type\":\"TRADE\"}/n{\"price\":\"11.32\",\"quantity\":100,\"type\":\"TRADE\"}/n[{\"type\":\"DEPTH_UPDATE\"},{\"bid\":[\"{\\\"action\\\":\\\"ADD\\\",\\\"price\\\":\\\"11.32\\\",\\\"quantity\\\":100}\",\"{\\\"action\\\":\\\"MODIFY\\\",\\\"price\\\":\\\"14.32\\\",\\\"quantity\\\":400}\"]},null]"));
        }

    } // namespace
} // namespace fep::src::feed_event
