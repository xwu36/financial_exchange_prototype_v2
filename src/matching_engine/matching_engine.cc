#include "src/matching_engine/matching_engine.h"

#include <iostream>

#include "lib/timestamp.h"

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
        using ::fep::src::order::OrderType;
        using ::fep::src::order::PriceEntity;

        // Return true if replenish of the order triggers.
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

            // TODO(): hardcode the replenish mechanish for now.
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

            order->timestamp_sec = fep::lib::now_in_secs();
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

        void UpdateOrderTradeEvent(const Price4 &price, const int32_t quantity,
                                   std::vector<OrderTradeEvent> &order_trade_events)
        {
            order_trade_events.push_back(OrderTradeEvent{
                .price = price,
                .quantity = quantity});
        }

        std::shared_ptr<PriceEntityUpdateEvent> GetUpdateEvent(const Price4 &price, const OrderSide &side,
                                                               const int32_t pre_quantity, const int32_t post_quantity)
        {
            if (pre_quantity == post_quantity)
            {
                return nullptr;
            }
            if (post_quantity == 0)
            {
                return std::make_shared<PriceEntityDeleteEvent>(price, post_quantity, side);
            }
            if (pre_quantity == 0)
            {
                return std::make_shared<PriceEntityAddEvent>(price, post_quantity, side);
            }
            return std::make_shared<PriceEntityModifyEvent>(price, post_quantity, side);
        }

        template <class T>
        void UpdateDepthUpdateEvents(const std::shared_ptr<Order> new_order,
                                     const int32_t new_order_price_pre_quantity,
                                     const int32_t new_order_price_post_quantity,
                                     const std::unordered_map<Price4, int32_t> &seen_price_to_pre_quantity,
                                     OrderBook<T> &order_book,
                                     DepthUpdateEvents &depth_update_events)
        {
            std::shared_ptr<PriceEntityUpdateEvent> new_order_update_event = GetUpdateEvent(new_order->price, new_order->side,
                                                                                            new_order_price_pre_quantity, new_order_price_post_quantity);
            if (new_order_update_event != nullptr)
            {
                depth_update_events.events.push_back(new_order_update_event);
            }

            for (const auto &[price, pre_quantity] : seen_price_to_pre_quantity)
            {
                std::shared_ptr<PriceEntityUpdateEvent> update_event = GetUpdateEvent(price, (new_order->side == OrderSide::BUY ? OrderSide::SELL : OrderSide::BUY),
                                                                                      pre_quantity, order_book.GetQuantityForPrice(price));
                depth_update_events.events.push_back(update_event);
            }
        }

        template <class T, class U>
        FeedEvents ProcessInternal(std::shared_ptr<Order> new_order, std::unordered_map<int64_t, std::shared_ptr<Order>> &order_to_content_map,
                                   OrderBook<T> &order_book_to_match, OrderBook<U> &order_book_to_insert)
        {
            FeedEvents feed_events{};
            std::unordered_map<Price4, int32_t> seen_price_to_pre_quantity;
            const int32_t new_order_price_pre_quantity = order_book_to_insert.GetQuantityForPrice(new_order->price);
            int32_t new_order_price_post_quantity = 0;

            while (new_order->quantity != 0)
            {
                // Get the top offer and its price entity.
                std::shared_ptr<PriceEntity> first_price_entry = order_book_to_match.TopPriceEntity();
                std::shared_ptr<Order> first_order = (first_price_entry == nullptr) ? nullptr : first_price_entry->visible_queue.front();

                // If the new_order cannot be matched, add it into the orderbook.
                if (!order_book_to_match.MatchOrder(new_order, first_order))
                {
                    if (new_order->order_type == OrderType::MARKET && first_order == nullptr)
                    {
                        break;
                    }
                    order_to_content_map.insert({new_order->order_id, new_order});
                    std::shared_ptr<PriceEntity> price_entry = order_book_to_insert.InsertOrder(new_order);
                    new_order_price_post_quantity = price_entry->visible_quantity;
                    break;
                }

                // TODO: Check if first_price_entry is a nullptr, which shouldn't happen.

                // Add the price and its quantity of the new order into the price_map.
                seen_price_to_pre_quantity.insert({first_order->price, first_price_entry->visible_quantity});
                // If the new order can be matched at the best limit price.
                const int32_t min_quantity = std::min(new_order->quantity, first_order->quantity);
                new_order->quantity -= min_quantity;
                first_order->quantity -= min_quantity;
                first_price_entry->visible_quantity -= min_quantity;

                UpdateOrderTradeEvent(first_order->price, min_quantity, feed_events.order_trade_events);

                auto &visible_queue = first_price_entry->visible_queue;
                // If replenish happens, pop out and push back the first order to keep the right order.
                if (MaybeReplenish(first_order))
                {
                    visible_queue.pop_front();
                    visible_queue.push_back(first_order);
                }
                if (MaybeMarkAsDeleted(first_order))
                {
                    order_to_content_map.erase(first_order->order_id);
                }
            }

            UpdateDepthUpdateEvents(new_order,
                                    new_order_price_pre_quantity,
                                    new_order_price_post_quantity,
                                    seen_price_to_pre_quantity,
                                    order_book_to_match,
                                    feed_events.depth_update_events);

            return feed_events;
        }

        template <class T>
        void UpdateQuantitiesAfterOrderCancelling(const std::shared_ptr<Order> order, OrderBook<T> &order_book,
                                                  int32_t &price_pre_quantity, int32_t &price_post_quantity)
        {
            price_pre_quantity = order_book.GetQuantityForPrice(order->price);
            order_book.MaybeCancelOrder(order);
            price_post_quantity = order_book.GetQuantityForPrice(order->price);
        }
    }

    absl::StatusOr<FeedEvents> MatchingEngine::Process(std::shared_ptr<Order> order)
    {
        // TODO: check if an offer is valid.

        if (order->type == OrderStatus::CANCEL)
        {
            return Cancel(order);
        }
        if (order->side == OrderSide::BUY)
        {
            return ProcessInternal(order, order_to_content_map_, ask_order_books_[order->symbol], bid_order_books_[order->symbol]);
        }
        if (order->side == OrderSide::SELL)
        {
            return ProcessInternal(order, order_to_content_map_, bid_order_books_[order->symbol], ask_order_books_[order->symbol]);
        }
        return absl::InvalidArgumentError("this order cannot be processed");
    }

    absl::StatusOr<FeedEvents> MatchingEngine::Cancel(std::shared_ptr<Order> order)
    {
        const auto kv = order_to_content_map_.find(order->order_id);
        if (kv == order_to_content_map_.end() || kv->second->deleted)
        {
            return absl::NotFoundError(absl::StrCat("Failed to cancel order ", order->order_id));
        }

        const auto detailed_order = kv->second;
        int32_t price_pre_quantity = 0;
        int32_t price_post_quantity = 0;
        if (detailed_order->side == OrderSide::SELL)
        {
            UpdateQuantitiesAfterOrderCancelling(detailed_order, ask_order_books_[detailed_order->symbol], price_pre_quantity, price_post_quantity);
        }
        else
        {
            UpdateQuantitiesAfterOrderCancelling(detailed_order, bid_order_books_[detailed_order->symbol], price_pre_quantity, price_post_quantity);
        }

        std::shared_ptr<PriceEntityUpdateEvent> update_event = GetUpdateEvent(detailed_order->price, detailed_order->side, price_pre_quantity, price_post_quantity);
        FeedEvents events;
        events.depth_update_events.events.push_back(update_event);

        kv->second->deleted = true;
        order_to_content_map_.erase(order->order_id);
        return events;
    }

} // namespace fep::src::matching_engine