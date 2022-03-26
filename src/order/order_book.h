#ifndef SRC_ORDER_ORDER_BOOK_H_
#define SRC_ORDER_ORDER_BOOK_H_

#include <iostream>
#include <memory>
#include <queue>

#include "lib/price4.h"
#include "src/order/order.h"

namespace fep::src::order
{

    struct PriceEntry
    {
        fep::lib::Price4 price;
        std::priority_queue<std::shared_ptr<Order>> visible_queue;
        std::priority_queue<std::shared_ptr<Order>> hidden_queue;
    };

    struct BidComparator
    {
        bool operator()(const PriceEntry &lhs, const PriceEntry &rhs)
        {
            return lhs.price > rhs.price;
        }
    };

    struct AskComparator
    {
        bool operator()(const PriceEntry &lhs, const PriceEntry &rhs)
        {
            return lhs.price < rhs.price;
        }
    };

    template <class T>
    class OrderBook
    {
    public:
        OrderBook() = default;
        OrderBook(const OrderBook &) = delete;
        OrderBook(OrderBook &&) = delete;

        void Process(std::shared_ptr<Order> order)
        {
            std::cout << "process..." << std::endl;
        }

    private:
        PriceEntry price_entry_;
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