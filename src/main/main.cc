#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
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

std::mutex mu;
std::condition_variable cond;
bool events_published = true;
bool market_off = false;
FeedEvents g_events;

void RunMatchingEngine()
{
  MatchingEngine matching_engine;
  std::vector<Order> orders = fep::src::util::ReadOrdersFromPath("src/main/data/orders.jsonl");
  // Loop through all the offers and process each of them.
  for (const Order &order : orders)
  {
    std::unique_lock<std::mutex> locker(mu);
    cond.wait(locker, []
              { return events_published; });

    matching_engine.Run(std::make_shared<Order>(order),
                        [&](absl::StatusOr<FeedEvents> feed_events)
                        {
                          if (feed_events.ok())
                          {
                            g_events = feed_events.value();
                          }
                          else
                          {
                            // TODO if feed_evnets not okay
                            g_events = FeedEvents{};
                          }

                          events_published = false;
                          locker.unlock();
                          cond.notify_one();
                        });

    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  market_off = true;
}

void RunPublisher()
{
  EventsPublisher events_publisher;
  while (!market_off)
  {
    std::unique_lock<std::mutex> locker(mu);
    cond.wait(locker, []()
              { return !events_published; });

    events_publisher.Publish(g_events);
    events_published = true;

    locker.unlock();
    cond.notify_one();
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
}

void Run()
{
  std::thread t1(RunMatchingEngine);
  std::thread t2(RunPublisher);

  t1.join();
  t2.join();
}

int main()
{
  Run();
  return 0;
}