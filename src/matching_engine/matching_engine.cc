#include "src/matching_engine/matching_engine.h"

namespace fep::src::matching_engine
{

    namespace
    {
        using ::fep::src::order::Order;
        using ::fep::src::order::OrderSide;
        using ::fep::src::order::OrderStatus;
        using ::fep::src::order::PriceEntity;

        // Return true if replenish of the order triggers.
        // TODO(): hardcode the replenish mechanish for now.
        bool MaybeReplenish(std::shared_ptr<Order> order)
        {
            if (order->quantity != 0)
            {
                return false;
            }
            if (order->hidden_quantity == 0)
            {
                return false;
            }
            if (order->hidden_quantity <= 100)
            {
                order->quantity = order->hidden_quantity;
                order->hidden_quantity = 0;
            }
            else
            {
                order->quantity = (order->quantity + order->hidden_quantity) / 2;
                order->hidden_quantity = order->hidden_quantity - order->quantity;
            }
            // TODO: update the timestamp.
            order->timestamp_sec = 1111111;
            return true;
        }

        // Return true if both visible and hidden quantity of the order are 0.
        bool MaybeMarkAsDeleted(std::shared_ptr<Order> order)
        {
            if (order->quantity == 0 && order->hidden_quantity == 0)
            {
                order->deleted = true;
                return true;
            }
            return false;
        }

        void UpdateOrderTradeEvent(const fep::lib::Price4 &price,
                                   const int32_t quantity, std::vector<fep::src::feed_event::OrderTradeEvent> &order_trade_events)
        {
            order_trade_events.push_back(fep::src::feed_event::OrderTradeEvent{
                .price = price,
                .quantity = quantity});
        }

        template <class T>
        void UpdateDepthUpdateEvents(const bool buy,
                                     const std::unordered_map<fep::lib::Price4, int32_t> &seen_price_to_pre_quantity,
                                     fep::src::feed_event::DepthUpdateEvents &depth_update_events,
                                     fep::src::order::OrderBook<T> &order_book)
        {
            for (const auto &[price, pre_quantity] : seen_price_to_pre_quantity)
            {
                const int32_t post_quantity = order_book.GetQuantityForPrice(price);
                if (pre_quantity == post_quantity)
                {
                    continue;
                }

                std::shared_ptr<fep::src::feed_event::PriceEntityUpdateEvent> update_event = nullptr;
                if (post_quantity == 0)
                {
                    update_event = std::make_shared<fep::src::feed_event::PriceEntityDeleteEvent>(price, post_quantity);
                }
                else if (pre_quantity == 0)
                {
                    update_event = std::make_shared<fep::src::feed_event::PriceEntityAddEvent>(price, post_quantity);
                }
                else
                {
                    update_event = std::make_shared<fep::src::feed_event::PriceEntityModifyEvent>(price, post_quantity);
                }

                if ((buy && post_quantity > pre_quantity) || (!buy && post_quantity < pre_quantity))
                {
                    depth_update_events.bid_events.push_back(std::move(update_event));
                }
                else
                {
                    depth_update_events.ask_events.push_back(std::move(update_event));
                }
            }
        }
    }

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
            auto &bid_order_book = bid_order_books_[order->symbol];
            return Sell(order, bid_order_book);
        }
        return events;
    }

    template <class T>
    fep::src::feed_event::FeedEvents MatchingEngine::Sell(std::shared_ptr<fep::src::order::Order> new_order,
                                                          fep::src::order::OrderBook<T> &order_book)
    {
        fep::src::feed_event::FeedEvents feed_events{};
        std::unordered_map<fep::lib::Price4, int32_t> seen_price_to_pre_quantity;
        seen_price_to_pre_quantity.insert({new_order->price, order_book.GetQuantityForPrice(new_order->price)});

        while (new_order->quantity != 0)
        {
            // Get the top offer and its price entity.
            std::shared_ptr<PriceEntity> first_price_entry = order_book.TopPriceEntity();
            auto &visible_queue = first_price_entry->visible_queue;
            std::shared_ptr<Order> first_order = visible_queue.front();

            // Add the price and its quantity of the new order into the price_map.
            seen_price_to_pre_quantity.insert({first_order->price, order_book.GetQuantityForPrice(first_order->price)});

            // If the new_order cannot be matched, add it into the orderbook.
            if (!order_book.MatchOrder(new_order, first_order))
            {
                const bool inserted = order_book.InsertPrice(new_order->price);
                auto price_entry = order_book.GetPriceEntity(new_order->price);
                price_entry->visible_queue.push_back(new_order);
                // If it is a new price_entry, add it to the queue.
                if (inserted)
                {
                    order_book.PushPriceEntityToQueue(price_entry);
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
                                feed_events.depth_update_events, order_book);

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