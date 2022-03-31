#include "src/matching_engine/matching_engine.h"

namespace fep::src::matching_engine
{

    namespace
    {
        using ::fep::lib::Price4;
        using ::fep::src::feed_event::DepthUpdateEvents;
        using ::fep::src::feed_event::FeedEvents;
        using ::fep::src::feed_event::OrderTradeEvent;
        using ::fep::src::feed_event::PriceEntityAddEvent;
        using ::fep::src::feed_event::PriceEntityDeleteEvent;
        using ::fep::src::feed_event::PriceEntityModifyEvent;
        using ::fep::src::feed_event::PriceEntityUpdateEvent;
        using ::fep::src::order::Order;
        using ::fep::src::order::OrderBook;
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

        void UpdateOrderTradeEvent(const Price4 &price,
                                   const int32_t quantity, std::vector<OrderTradeEvent> &order_trade_events)
        {
            order_trade_events.push_back(OrderTradeEvent{
                .price = price,
                .quantity = quantity});
        }

        std::shared_ptr<PriceEntityUpdateEvent> GetUpdateEvent(const Price4 &price, const int32_t pre_quantity,
                                                               const int32_t post_quantity)
        {
            if (pre_quantity == post_quantity)
            {
                return nullptr;
            }
            if (post_quantity == 0)
            {
                return std::make_shared<PriceEntityDeleteEvent>(price, post_quantity);
            }
            if (pre_quantity == 0)
            {
                return std::make_shared<PriceEntityAddEvent>(price, post_quantity);
            }
            return std::make_shared<PriceEntityModifyEvent>(price, post_quantity);
        }

        template <class T>
        void UpdateDepthUpdateEvents(const bool buy,
                                     const Price4 &new_order_price,
                                     const int32_t new_order_pre_quantity,
                                     const int32_t new_order_post_quantity,
                                     const std::unordered_map<Price4, int32_t> &seen_price_to_pre_quantity,
                                     DepthUpdateEvents &depth_update_events,
                                     OrderBook<T> &order_book)
        {
            std::shared_ptr<PriceEntityUpdateEvent> new_order_update_event = GetUpdateEvent(new_order_price, new_order_pre_quantity,
                                                                                            new_order_post_quantity);
            if (new_order_update_event != nullptr)
            {
                if (buy)
                    depth_update_events.bid_events.push_back(new_order_update_event);
                else
                    depth_update_events.ask_events.push_back(new_order_update_event);
            }

            for (const auto &[price, pre_quantity] : seen_price_to_pre_quantity)
            {
                std::shared_ptr<PriceEntityUpdateEvent> update_event = GetUpdateEvent(price, pre_quantity,
                                                                                      order_book.GetQuantityForPrice(price));
                if (buy)
                    depth_update_events.ask_events.push_back(update_event);
                else
                    depth_update_events.bid_events.push_back(update_event);
            }
        }

    }

    FeedEvents MatchingEngine::Process(std::shared_ptr<Order> order)
    {
        // TODO: check if an offer is valid.
        FeedEvents events;
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
            return Process(order, ask_order_books_[order->symbol], bid_order_books_[order->symbol]);
        }
        if (order->side == OrderSide::SELL)
        {
            return Process(order, bid_order_books_[order->symbol], ask_order_books_[order->symbol]);
        }
        return events;
    }

    template <class T, class U>
    FeedEvents MatchingEngine::Process(std::shared_ptr<Order> new_order, OrderBook<T> &order_book_to_match,
                                       OrderBook<U> &order_book_to_insert)
    {
        FeedEvents feed_events{};
        std::unordered_map<Price4, int32_t> seen_price_to_pre_quantity;
        const int32_t new_order_pre_quantity = order_book_to_insert.GetQuantityForPrice(new_order->price);

        while (new_order->quantity != 0)
        {
            // Get the top offer and its price entity.
            std::shared_ptr<PriceEntity> first_price_entry = order_book_to_match.TopPriceEntity();
            auto &visible_queue = first_price_entry->visible_queue;
            std::shared_ptr<Order> first_order = visible_queue.front();

            // Add the price and its quantity of the new order into the price_map.
            seen_price_to_pre_quantity.insert({first_order->price, order_book_to_match.GetQuantityForPrice(first_order->price)});

            // If the new_order cannot be matched, add it into the orderbook.
            if (!order_book_to_match.MatchOrder(new_order, first_order))
            {
                const bool inserted = order_book_to_insert.InsertPrice(new_order->price);
                std::shared_ptr<PriceEntity> price_entry = order_book_to_insert.GetPriceEntity(new_order->price);
                price_entry->visible_queue.push_back(new_order);
                // If it is a new price_entry, add it to the queue.
                if (inserted)
                {
                    order_book_to_insert.PushPriceEntityToQueue(price_entry);
                }
                price_entry->visible_quantity += new_order->quantity;
                break;
            }

            // If the new order can be matched at the best limit price.
            const int32_t min_quantity = std::min(new_order->quantity, first_order->quantity);
            new_order->quantity -= min_quantity;
            first_order->quantity -= min_quantity;

            UpdateOrderTradeEvent(first_order->price, min_quantity, feed_events.order_trade_events);

            // If replenish happens, pop out and push back the first order to keep the right order.
            if (MaybeReplenish(first_order))
            {
                visible_queue.pop_front();
                visible_queue.push_back(first_order);
            }
            MaybeMarkAsDeleted(first_order);
        }

        UpdateDepthUpdateEvents(new_order->side == OrderSide::BUY,
                                new_order->price,
                                new_order_pre_quantity,
                                new_order->quantity,
                                seen_price_to_pre_quantity,
                                feed_events.depth_update_events, order_book_to_match);

        return feed_events;
    }

    FeedEvents MatchingEngine::Cancel(std::shared_ptr<Order> order)
    {
        FeedEvents events;
        return events;
    }

} // namespace fep::src::matching_engine