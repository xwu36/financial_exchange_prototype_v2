#ifndef SRC_FEED_EVENT_FEED_EVENT_H_
#define SRC_FEED_EVENT_FEED_EVENT_H_

#include <memory>
#include <string>
#include <vector>

#include "lib/price4.h"
#include "nlohmann/json.hpp"

namespace fep::src::feed_event
{
    struct OrderTradeEvent
    {
        const std::string type = "TRADE";
        fep::lib::Price4 price;
        int32_t quantity = 0;

        std::string to_str() const;
    };

    struct PriceEntityUpdateEvent
    {
        PriceEntityUpdateEvent(const fep::lib::Price4 &in_price,
                               int32_t in_quantity,
                               const std::string &in_action) : price(in_price), quantity(in_quantity), action(in_action) {}

        const std::string &get_action() const { return action; }
        std::string to_str() const;

        fep::lib::Price4 price;
        int32_t quantity = 0;
        std::string action;
    };
    struct PriceEntityAddEvent : public PriceEntityUpdateEvent
    {
        PriceEntityAddEvent(const fep::lib::Price4 &price,
                            int32_t quantity) : PriceEntityUpdateEvent(price, quantity, "ADD") {}
    };
    struct PriceEntityDeleteEvent : public PriceEntityUpdateEvent
    {
        PriceEntityDeleteEvent(const fep::lib::Price4 &price,
                               int32_t quantity) : PriceEntityUpdateEvent(price, quantity, "DELETE") {}
    };
    struct PriceEntityModifyEvent : public PriceEntityUpdateEvent
    {
        PriceEntityModifyEvent(const fep::lib::Price4 &price,
                               int32_t quantity) : PriceEntityUpdateEvent(price, quantity, "MODIFY") {}
    };

    struct DepthUpdateEvents
    {
        const std::string type = "DEPTH_UPDATE";
        std::vector<std::shared_ptr<PriceEntityUpdateEvent>> bid_events;
        std::vector<std::shared_ptr<PriceEntityUpdateEvent>> ask_events;

        std::string to_str() const;
    };

    struct FeedEvents
    {
        std::vector<OrderTradeEvent> order_trade_events;
        DepthUpdateEvents depth_update_events;

        std::string to_str() const;
    };

} // namespace fep::src::feed_event

#endif