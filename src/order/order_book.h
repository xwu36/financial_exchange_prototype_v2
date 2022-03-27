#ifndef SRC_ORDER_ORDER_BOOK_H_
#define SRC_ORDER_ORDER_BOOK_H_

#include <cmath>
#include <deque>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "lib/price4.h"
#include "src/feed_event/feed_event.h"
#include "src/order/order.h"

// TODO this file needs to be tested.
namespace fep::src::order
{

    struct PriceEntity
    {
        fep::lib::Price4 price;
        mutable std::deque<std::shared_ptr<Order>> visible_queue;
    };

    struct BidComparator
    {
        bool operator()(const std::shared_ptr<PriceEntity> &lhs, const std::shared_ptr<PriceEntity> &rhs)
        {
            return lhs->price > rhs->price;
        }
    };

    struct AskComparator
    {
        bool operator()(const std::shared_ptr<PriceEntity> &lhs, const std::shared_ptr<PriceEntity> &rhs)
        {
            return lhs->price < rhs->price;
        }
    };

    template <class T>
    class OrderBook
    {
    public:
        OrderBook() = default;
        OrderBook(const OrderBook &) = delete;
        OrderBook(OrderBook &&) = delete;

        // Process a new order or a cancel order.
        // The validity of the order should be done in the upstream.
        fep::src::feed_event::FeedEvents Process(std::shared_ptr<Order> new_order)
        {
            if (new_order->type == OrderStatus::NEW)
            {
                return HandleNewOrder(new_order);
            }
            else if (new_order->type == OrderStatus::CANCEL)
            {
                return HandleCancelOrder(new_order->order_id);
            }

            // TODO
            return fep::src::feed_event::FeedEvents{};
        }

    protected:
        fep::src::feed_event::FeedEvents HandleNewOrder(std::shared_ptr<Order> new_order)
        {
            fep::src::feed_event::FeedEvents feed_events{};
            std::unordered_map<fep::lib::Price4, int32_t> seen_price_to_pre_quantity;
            seen_price_to_pre_quantity.insert({new_order->price, price_to_visible_quantity_map_[new_order->price]});

            while (new_order->quantity != 0)
            {
                std::shared_ptr<PriceEntity> first_price_entry = TopPriceEntity();
                auto &visible_queue = first_price_entry->visible_queue;
                std::shared_ptr<Order> first_order = visible_queue.front();
                seen_price_to_pre_quantity.insert({first_order->price, price_to_visible_quantity_map_[first_order->price]});
                if (!MatchOrder(new_order, first_order))
                {
                    const auto &itr = price_to_entry_map_.insert({new_order->price, std::make_shared<PriceEntity>()});
                    auto &price_entry = itr.first->second;
                    price_entry->visible_queue.push_back(new_order);
                    // If it is a new price_entry, add it to the queue.
                    if (itr.second)
                    {
                        price_queue_.push(price_entry);
                    }
                    price_to_visible_quantity_map_[new_order->price] += new_order->quantity;
                    break;
                }

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

        // TODO
        fep::src::feed_event::FeedEvents HandleCancelOrder(const int64_t order_id)
        {
            deleted_order_ids_.insert(order_id);
            // TODO
            return fep::src::feed_event::FeedEvents{};
        }

        // Return true if the input order matches the target order.
        // Orders match when the input order is a maket order.
        // Orders match when the input order is a sell order, and its price lower than the target order.
        // Orders match when the input order is a buy order, and its price higher than the target order.
        bool MatchOrder(std::shared_ptr<Order> input_order, std::shared_ptr<Order> target_order)
        {
            if (target_order == nullptr)
            {
                return false;
            }
            if (input_order->order_type == OrderType::MARKET)
            {
                return true;
            }
            return (input_order->side == OrderSide::SELL) ? target_order->price >= input_order->price : target_order->price <= input_order->price;
        }

        // Return the top price entity in the order book.
        // The top order of the entity is guarateed to be an un-deleted order.
        // Remove deleted orders/empty entities if they are on the top until the first un-deleted order is met.
        std::shared_ptr<PriceEntity> TopPriceEntity()
        {
            while (!price_queue_.empty())
            {
                auto &visible_queue = price_queue_.top()->visible_queue;
                while (!visible_queue.empty())
                {
                    const auto &first_order = visible_queue.front();
                    if (first_order->deleted || deleted_order_ids_.count(first_order->order_id))
                    {
                        deleted_order_ids_.erase(first_order->order_id);
                        visible_queue.pop_front();
                        continue;
                    }
                    return price_queue_.top();
                }
                price_to_entry_map_.erase(price_queue_.top()->price);
                price_queue_.pop();
            }
            return nullptr;
        }

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

        void UpdateDepthUpdateEvents(const bool buy,
                                     const std::unordered_map<fep::lib::Price4, int32_t> &seen_price_to_pre_quantity,
                                     fep::src::feed_event::DepthUpdateEvents &depth_update_events)
        {
            for (const auto &[price, pre_quantity] : seen_price_to_pre_quantity)
            {
                const int32_t post_quantity = price_to_visible_quantity_map_[price];
                if (pre_quantity == post_quantity)
                {
                    continue;
                }

                std::unique_ptr<fep::src::feed_event::PriceEntityUpdateEvent> update_event = nullptr;
                if (post_quantity == 0)
                {
                    update_event = std::make_unique<fep::src::feed_event::PriceEntityDeleteEvent>();
                }
                else if (pre_quantity == 0)
                {
                    update_event = std::make_unique<fep::src::feed_event::PriceEntityAddEvent>();
                }
                else
                {
                    update_event = std::make_unique<fep::src::feed_event::PriceEntityModifyEvent>();
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

        std::priority_queue<std::shared_ptr<PriceEntity>> price_queue_;
        std::unordered_map<fep::lib::Price4, std::shared_ptr<PriceEntity>> price_to_entry_map_;
        std::unordered_set<int64_t> deleted_order_ids_;
        std::unordered_map<fep::lib::Price4, int32_t> price_to_visible_quantity_map_;
    };

    class BidOrderBook : public OrderBook<BidComparator>
    {
    public:
        BidOrderBook() = default;
        BidOrderBook(const BidOrderBook &) = delete;
        BidOrderBook(BidOrderBook &&) = delete;
    };

    class AskOrderBook : public OrderBook<AskOrderBook>
    {
    public:
        AskOrderBook() = default;
        AskOrderBook(const AskOrderBook &) = delete;
        AskOrderBook(AskOrderBook &&) = delete;
    };

} // fep::src::order

#endif