#include <iostream>
#include <memory>
#include <iostream>
#include <vector>

#include "external/com_google_absl/absl/status/statusor.h"
#include "nlohmann/json.hpp"
#include "src/order/order.h"
#include "src/util/orders_reader.h"
#include "src/feed_event/feed_event.h"
#include "src/publisher/events_publisher.h"
#include "src/matching_engine/matching_engine.h"

using ::fep::src::feed_event::FeedEvents;
using ::fep::src::matching_engine::MatchingEngine;
using ::fep::src::order::Order;
using ::fep::src::publisher::EventsPublisher;
using ::nlohmann::json;

int main()
{
  MatchingEngine matching_engine;
  EventsPublisher events_publisher;
  // Process today's orders.
  std::vector<Order> orders = fep::src::util::ReadOrdersFromPath("src/main/data/orders.jsonl");
  // Loop through all the offers and process each of them.
  for (const Order &order : orders)
  {
    absl::StatusOr<FeedEvents> message = matching_engine.Process(std::make_shared<Order>(order));
    if (message.ok())
    {
      // TOOD: handle by multiple threads
      events_publisher.Publish(message.value());
    }
  }

  return 0;
}