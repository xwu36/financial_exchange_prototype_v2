#ifndef SRC_MATCHING_ENGINE_MATCHING_ENGINE_H_
#define SRC_MATCHING_ENGINE_MATCHING_ENGINE_H_

#include <functional>
#include <unordered_map>

#include "external/com_google_absl/absl/status/statusor.h"
#include "src/feed_event/feed_event.h"
#include "src/order/order.h"
#include "src/order/order_book.h"
#include "src/stock/symbol.h"

namespace fep::src::matching_engine
{

  class MatchingEngine
  {
  public:
    typedef std::function<void(absl::StatusOr<fep::src::feed_event::FeedEvents>)> Callback;

    MatchingEngine() {}
    MatchingEngine(const MatchingEngine &) = delete;
    MatchingEngine(MatchingEngine &&) = delete;
    ~MatchingEngine() = default;

    absl::StatusOr<fep::src::feed_event::FeedEvents> Process(std::shared_ptr<fep::src::order::Order> order);
    void Run(std::shared_ptr<fep::src::order::Order> order, Callback callback)
    {
      absl::StatusOr<fep::src::feed_event::FeedEvents> feed_events = Process(order);
      callback(feed_events);
    }

  private:
    absl::StatusOr<fep::src::feed_event::FeedEvents> Cancel(std::shared_ptr<fep::src::order::Order> order);

    std::unordered_map<fep::src::stock::Symbol, fep::src::order::BidOrderBook> bid_order_books_;
    std::unordered_map<fep::src::stock::Symbol, fep::src::order::AskOrderBook> ask_order_books_;
    std::unordered_map<int64_t /*order_id*/, std::shared_ptr<fep::src::order::Order>> order_to_content_map_;
  };

} // namespace fep::src::matching_engine
#endif