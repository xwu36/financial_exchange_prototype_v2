#ifndef SRC_ORDER_ORDER_BOOK_H_
#define SRC_ORDER_ORDER_BOOK_H_

#include <deque>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "lib/price4.h"
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
        void Process(std::shared_ptr<Order> new_order)
        {
            if (new_order->type == OrderStatus::NEW)
            {
                HandleNewOrder(new_order);
            }
            else if (new_order->type == OrderStatus::CANCEL)
            {
                HandleCancelOrder(new_order);
            }
        }

    protected:
        void HandleNewOrder(std::shared_ptr<Order> new_order)
        {
            while (new_order->quantity != 0)
            {
                std::shared_ptr<PriceEntity> first_price_entry = TopPriceEntity();
                auto &visible_queue = first_price_entry->visible_queue;
                std::shared_ptr<Order> first_order = visible_queue.front();
                if (!MatchOrder(new_order, first_order))
                {
                    const auto &kv = price_to_entry_map_.find(new_order->price);
                    if (kv == price_to_entry_map_.end())
                    {
                        kv->second = std::make_shared<PriceEntity>();
                        price_queue_.push(kv->second);
                        deleted_order_ids_.insert(new_order->order_id);
                    }
                    kv->second->visible_queue.push_back(new_order);
                    return;
                }

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
                // If replenish happens, pop out and push back the first order to keep the right order.
                if (MaybeReplenish(first_order))
                {
                    visible_queue.pop_front();
                    visible_queue.push_back(first_order);
                }
                // pop out the first order if it is marked as deleted.
                if (MaybeMarkAsDeleted(first_order))
                {
                    visible_queue.pop_front();
                }
            }
        }

        // TODO
        void HandleCancelOrder(std::shared_ptr<Order> order)
        {
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
                    if (visible_queue.front()->deleted)
                    {
                        deleted_order_ids_.erase(visible_queue.front()->order_id);
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