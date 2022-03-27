#ifndef SRC_FEED_EVENT_FEED_EVENT_H_
#define SRC_FEED_EVENT_FEED_EVENT_H_

#include <string>
#include <vector>

#include "lib/price4.h"

namespace fep::src::feed_event
{
    struct OrderTradeEvent
    {
        const std::string trade = "TRADE";
        fep::lib::Price4 price;
        int32_t quantity = 0;
    };

    struct PriceEntityUpdateEvent
    {
        fep::lib::Price4 price;
        int32_t quantity = 0;
        std::string action;
    };
    struct PriceEntityAddEvent : PriceEntityUpdateEvent
    {
        std::string action = "ADD";
    };
    struct PriceEntityDeleteEvent : PriceEntityUpdateEvent
    {
        std::string action = "Delete";
    };
    struct PriceEntityModifyEvent : PriceEntityUpdateEvent
    {
        std::string action = "Modify";
    };

    struct DepthUpdateEvents
    {
        const std::string type = "DEPTH_UPDATE";
        std::vector<std::unique_ptr<PriceEntityUpdateEvent>> bid_events;
        std::vector<std::unique_ptr<PriceEntityUpdateEvent>> ask_events;
    };

    struct FeedEvents
    {
        std::vector<OrderTradeEvent> order_trade_events;
        DepthUpdateEvents depth_update_events;
    };

} // namespace fep::src::feed_event

#endif