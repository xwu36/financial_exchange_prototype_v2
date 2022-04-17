#ifndef SRC_MATCHING_ENGINE_MATCHING_ENGINE_H_
#define SRC_MATCHING_ENGINE_MATCHING_ENGINE_H_

#include <functional>
#include <unordered_map>

#include "external/com_google_absl/absl/status/status.h"
#include "external/com_google_absl/absl/status/statusor.h"
#include "src/feed_event/feed_event.h"
#include "src/matching_engine/tick_size_rule.h"
#include "src/order/order.h"
#include "src/order/order_book.h"
#include "src/stock/symbol.h"

namespace fep::src::matching_engine
{

  class MatchingEngine
  {
  public:
    typedef std::function<void(absl::StatusOr<fep::src::feed_event::FeedEvents>)> Callback;

    MatchingEngine();
    MatchingEngine(const MatchingEngine &) = delete;
    MatchingEngine(MatchingEngine &&) = delete;
    ~MatchingEngine() = default;

    absl::StatusOr<fep::src::feed_event::FeedEvents> Process(std::shared_ptr<fep::src::order::Order> order);
    // Process a new order and notify the publiser.
    void ProcessAndNotify(std::shared_ptr<fep::src::order::Order> order, Callback callback)
    {
      absl::StatusOr<fep::src::feed_event::FeedEvents> feed_events = Process(order);
      callback(feed_events);
    }
    // Initialize orderbooks and orderpool when market starts using the unmatched orders yesterday.
    absl::Status InitOnMarketStarts(const std::string &order_path_a_day_ago);
    // Clear orderbooks and orderpool when market ends.
    absl::Status ClearOnMarketEnds(const std::string &output_path);

  private:
    // Cancel an order.
    absl::StatusOr<fep::src::feed_event::FeedEvents> Cancel(std::shared_ptr<fep::src::order::Order> order);
    // Check if an offer is valid.
    absl::Status IsOfferValid(const std::shared_ptr<fep::src::order::Order> order) const;

    std::unordered_map<fep::src::stock::Symbol, fep::src::order::BidOrderBook> bid_order_books_;
    std::unordered_map<fep::src::stock::Symbol, fep::src::order::AskOrderBook> ask_order_books_;
    std::unordered_map<int64_t /*order_id*/, std::shared_ptr<fep::src::order::Order>> order_to_content_map_;
    uint32_t lot_size_;
    TickSizeRule tick_size_rule_;
  };

} // namespace fep::src::matching_engine
#endif