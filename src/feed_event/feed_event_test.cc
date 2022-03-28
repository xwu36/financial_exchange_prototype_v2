#include "src/feed_event/feed_event.h"

#include <memory>

#include "gtest/gtest.h"

namespace fep::src::feed_event
{
    namespace
    {
        using fep::lib::Price4;

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
