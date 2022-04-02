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

namespace fep::src::order
{

    struct PriceEntity
    {
        PriceEntity(const fep::lib::Price4 &in_price) : price(in_price) {}

        fep::lib::Price4 price;
        mutable std::deque<std::shared_ptr<Order>> visible_queue;
        int32_t visible_quantity = 0;
    };

    struct BidComparator
    {
        bool operator()(const std::shared_ptr<PriceEntity> &lhs, const std::shared_ptr<PriceEntity> &rhs) const
        {
            return lhs->price < rhs->price;
        }
    };

    struct AskComparator
    {
        bool operator()(const std::shared_ptr<PriceEntity> &lhs, const std::shared_ptr<PriceEntity> &rhs) const
        {
            return lhs->price > rhs->price;
        }
    };

    template <class T>
    class OrderBook
    {
    public:
        OrderBook() = default;
        OrderBook(const OrderBook &) = delete;
        OrderBook(OrderBook &&) = delete;

        // Return false if it is a new orer, otherwise mark the order deleted and return true.
        bool MaybeCancelOrder(std::shared_ptr<Order> order)
        {
            if (all_order_ids_.count(order->order_id))
            {
                return false;
            }
            deleted_order_ids_.insert(order->order_id);
            std::shared_ptr<PriceEntity> price_entity = this->GetPriceEntity(order);
            price_entity->visible_quantity -= order->quantity;
            return true;
        }

        // Return true if the input order matches the target order.
        virtual bool MatchOrder(std::shared_ptr<Order> input_order, std::shared_ptr<Order> target_order) const = 0;

        // Return the top price entity in the order book. The top entity could be a nullptr if there is no un-deleted order.
        // The top order of the entity is guarateed to be an un-deleted order if there is an un-deleted order.
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

        // Return visible quantity for a given price.
        // Quantity is 0 if no such price in the book.
        int32_t GetQuantityForPrice(const fep::lib::Price4 &price) const
        {
            const auto kv = price_to_entry_map_.find(price);
            return (kv == price_to_entry_map_.end()) ? 0 : kv->second->visible_quantity;
        }

        // Return the price entity for the given price.
        // If no such price, create a new price entity in the map.
        std::shared_ptr<PriceEntity> GetPriceEntity(const fep::lib::Price4 &price)
        {
            const auto &itr = price_to_entry_map_.insert({price, std::make_shared<PriceEntity>(price)});
            if (itr.second)
            {
                price_queue_.push(itr.first->second);
            }
            return itr.first->second;
        }

        // Insert order on the order_book.
        void InsertOrder(const std::shared_ptr<src::order::Order> order)
        {
            std::shared_ptr<PriceEntity> price_entry = this->GetPriceEntity(order->price);
            price_entry->visible_queue.push_back(order);
            price_entry->visible_quantity += order->quantity;
            all_order_ids_.insert(order->order_id);
        }

    private:
        std::priority_queue<std::shared_ptr<PriceEntity>, std::vector<std::shared_ptr<PriceEntity>>, T> price_queue_;
        std::unordered_map<fep::lib::Price4, std::shared_ptr<PriceEntity>> price_to_entry_map_;
        std::unordered_set<int64_t> deleted_order_ids_;
        std::unordered_set<int64_t> all_order_ids_;
    };

    class BidOrderBook : public OrderBook<BidComparator>
    {
    public:
        BidOrderBook() = default;
        BidOrderBook(const BidOrderBook &) = delete;
        BidOrderBook(BidOrderBook &&) = delete;

        bool MatchOrder(std::shared_ptr<Order> input_order, std::shared_ptr<Order> target_order) const override
        {
            if (target_order == nullptr)
            {
                return false;
            }
            if (input_order->order_type == OrderType::MARKET)
            {
                return true;
            }
            return target_order->price >= input_order->price;
        }
    };

    class AskOrderBook : public OrderBook<AskComparator>
    {
    public:
        AskOrderBook() = default;
        AskOrderBook(const AskOrderBook &) = delete;
        AskOrderBook(AskOrderBook &&) = delete;

        bool MatchOrder(std::shared_ptr<Order> input_order, std::shared_ptr<Order> target_order) const override
        {
            if (target_order == nullptr)
            {
                return false;
            }
            if (input_order->order_type == OrderType::MARKET)
            {
                return true;
            }
            return target_order->price <= input_order->price;
        }
    };

} // fep::src::order

#endif