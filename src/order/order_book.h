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
        int32_t visible_quantity;
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

        int32_t GetQuantityForPrice(const fep::lib::Price4 &price)
        {
            return price_to_entry_map_[price]->visible_quantity;
        }

        std::shared_ptr<PriceEntity> GetPriceEntity(const fep::lib::Price4 &price)
        {
            return price_to_entry_map_[price];
        }

        bool InsertPrice(const fep::lib::Price4 &price)
        {
            const auto &itr = price_to_entry_map_.insert({price, std::make_shared<PriceEntity>()});
            return itr.second;
        }

        void PushPriceEntityToQueue(const std::shared_ptr<PriceEntity> price_entity)
        {
            price_queue_.push(price_entity);
        }

        std::priority_queue<std::shared_ptr<PriceEntity>> price_queue_;
        std::unordered_map<fep::lib::Price4, std::shared_ptr<PriceEntity>> price_to_entry_map_;
        std::unordered_set<int64_t> deleted_order_ids_;
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