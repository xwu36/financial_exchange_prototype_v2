#include "src/order/order_book.h"

#include "gtest/gtest.h"

namespace fep::src::order
{
  namespace
  {

    TEST(OrderBookTest, ProcessOrder)
    {
      BidOrderBook orderbook;
      std::shared_ptr<Order> order = std::make_shared<Order>();
      orderbook.Process(order);
    }

  } // namespace
} // namespace fep::src::order