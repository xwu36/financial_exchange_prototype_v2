#ifndef SRC_FEED_EVENT_FEED_EVENT_H_
#define SRC_FEED_EVENT_FEED_EVENT_H_

#include <memory>
#include <string>
#include <vector>

#include "lib/price4.h"
#include "nlohmann/json.hpp"
#include "src/order/order.h"

namespace fep::src::feed_event
{
    struct OrderTradeEvent
    {
        fep::lib::Price4 price;
        int32_t quantity = 0;
        std::string type = "TRADE";

        std::string to_str() const;
    };

    struct PriceEntityUpdateEvent
    {
        PriceEntityUpdateEvent(const fep::lib::Price4 &in_price,
                               int32_t in_quantity,
                               const std::string &in_action,
                               src::order::OrderSide in_side) : price(in_price), quantity(in_quantity),
                                                                action(in_action), side(in_side) {}

        const std::string &get_action() const { return action; }
        std::string to_str() const;

        fep::lib::Price4 price;
        int32_t quantity = 0;
        std::string action;
        src::order::OrderSide side = src::order::OrderSide::UNKNOWN;
    };
    struct PriceEntityAddEvent : public PriceEntityUpdateEvent
    {
        PriceEntityAddEvent(const fep::lib::Price4 &price, int32_t quantity,
                            src::order::OrderSide side) : PriceEntityUpdateEvent(price, quantity, "ADD", side) {}
    };
    struct PriceEntityDeleteEvent : public PriceEntityUpdateEvent
    {
        PriceEntityDeleteEvent(const fep::lib::Price4 &price, int32_t quantity,
                               src::order::OrderSide side) : PriceEntityUpdateEvent(price, quantity, "DELETE", side) {}
    };
    struct PriceEntityModifyEvent : public PriceEntityUpdateEvent
    {
        PriceEntityModifyEvent(const fep::lib::Price4 &price, int32_t quantity,
                               src::order::OrderSide side) : PriceEntityUpdateEvent(price, quantity, "MODIFY", side) {}
    };

    struct DepthUpdateEvents
    {
        std::string type = "DEPTH_UPDATE";
        std::vector<std::shared_ptr<PriceEntityUpdateEvent>> events;

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