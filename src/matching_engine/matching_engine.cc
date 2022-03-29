#include "src/matching_engine/matching_engine.h"

namespace fep::src::matching_engine
{

    using ::fep::src::order::OrderSide;
    using ::fep::src::order::OrderStatus;

    fep::src::feed_event::FeedEvents MatchingEngine::Process(std::shared_ptr<fep::src::order::Order> order)
    {
        // TODO: check if an offer is valid.
        fep::src::feed_event::FeedEvents events;
        if (order->type == OrderStatus::UNKNOWN)
        {
            return events;
        }
        if (order->type == OrderStatus::CANCEL)
        {
            return Cancel(order);
        }
        if (order->side == OrderSide::BUY)
        {
            return Buy(order);
        }
        if (order->side == OrderSide::SELL)
        {
            return Sell(order);
        }
        return events;
    }

    fep::src::feed_event::FeedEvents MatchingEngine::Sell(std::shared_ptr<fep::src::order::Order> order)
    {
        fep::src::feed_event::FeedEvents feed_events{};
        std::unordered_map<fep::lib::Price4, int32_t> seen_price_to_pre_quantity;
        auto &bid_order_book = bid_order_books_[order->symbol];
        seen_price_to_pre_quantity.insert({new_order->price, price_to_entry_map_[new_order->price]->visible_quantity});

        while (new_order->quantity != 0)
        {
            // Get the top offer and its price entity.
            std::shared_ptr<PriceEntity> first_price_entry = bid_order_book.TopPriceEntity();
            auto &visible_queue = first_price_entry->visible_queue;
            std::shared_ptr<Order> first_order = visible_queue.front();

            // Add the price and its quantity of the new order into the price_map.
            seen_price_to_pre_quantity.insert({first_order->price, price_to_entry_map_[first_order->price]->visible_quantity});

            // If the new_order cannot be matched, add it into the orderbook.
            if (!bid_order_book.MatchOrder(new_order, first_order))
            {
                const auto &itr = price_to_entry_map_.insert({new_order->price, std::make_shared<PriceEntity>()});
                auto &price_entry = itr.first->second;
                price_entry->visible_queue.push_back(new_order);
                // If it is a new price_entry, add it to the queue.
                if (itr.second)
                {
                    price_queue_.push(price_entry);
                }
                price_entry->visible_quantity += new_order->quantity;
                break;
            }

            // If the new order can be matched at the best limit price.
            const int32_t first_order_pre_quantity = first_order->quantity;
            if (new_order->quantity >= first_order->quantity)
            {
                new_order->quantity -= first_order->quantity;
                first_order->quantity = 0;
            }
            else
            {
                first_order->quantity -= new_order->quantity;
                new_order->quantity = 0;
            }
            UpdateOrderTradeEvent(first_order->price, first_order_pre_quantity - first_order->quantity,
                                  feed_events.order_trade_events);

            // If replenish happens, pop out and push back the first order to keep the right order.
            if (MaybeReplenish(first_order))
            {
                visible_queue.pop_front();
                visible_queue.push_back(first_order);
            }
            MaybeMarkAsDeleted(first_order);
        }

        UpdateDepthUpdateEvents(new_order->side == OrderSide::BUY, seen_price_to_pre_quantity,
                                feed_events.depth_update_events);

        return feed_events;
    }

    fep::src::feed_event::FeedEvents MatchingEngine::Buy(std::shared_ptr<fep::src::order::Order> order)
    {
        fep::src::feed_event::FeedEvents events;
        return events;
    }

    fep::src::feed_event::FeedEvents MatchingEngine::Cancel(std::shared_ptr<fep::src::order::Order> order)
    {
        fep::src::feed_event::FeedEvents events;
        return events;
    }

} // namespace fep::src::matching_engine