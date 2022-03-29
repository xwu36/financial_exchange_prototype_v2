#ifndef SRC_MATCHING_ENGINE_MATCHING_ENGINE_H_
#define SRC_MATCHING_ENGINE_MATCHING_ENGINE_H_

#include <unordered_map>

#include "src/feed_event/feed_event.h"
#include "src/order/order.h"
#include "src/order/order_book.h"
#include "src/stock/symbol.h"

namespace fep::src::matching_engine
{

  class MatchingEngine
  {
  public:
    MatchingEngine() {}
    ~MatchingEngine() = default;
    fep::src::feed_event::FeedEvents Process(std::shared_ptr<fep::src::order::Order> order);

  protected:
    fep::src::feed_event::FeedEvents Sell(std::shared_ptr<fep::src::order::Order> order);
    fep::src::feed_event::FeedEvents Buy(std::shared_ptr<fep::src::order::Order> order);
    fep::src::feed_event::FeedEvents Cancel(std::shared_ptr<fep::src::order::Order> order);

    std::unordered_map<fep::src::stock::Symbol, fep::src::order::BidOrderBook> bid_order_books_;
    std::unordered_map<fep::src::stock::Symbol, fep::src::order::AskOrderBook> ask_order_books_;
  };

} // namespace fep::src::matching_engine
#endif