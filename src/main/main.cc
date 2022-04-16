#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>

#include "external/com_google_absl/absl/status/statusor.h"
#include "lib/timestamp.h"
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
std::deque<FeedEvents> buffer;
const unsigned int maxBufferSize = 2;

bool market_off = false;

void RunMatchingEngine()
{
  MatchingEngine matching_engine;
  std::vector<Order> orders = fep::src::util::ReadOrdersFromPath("src/main/data/orders.jsonl");
  // Loop through all the offers and process each of them.
  for (const Order &order : orders)
  {
    std::cout << "Receive new order at " << fep::lib::now_in_secs() << std::endl;
    std::unique_lock<std::mutex> locker(mu);
    // cond.wait(locker, []
    //           { return buffer.size() < maxBufferSize; });

    matching_engine.ProcessAndNotify(std::make_shared<Order>(order),
                                     [&](absl::StatusOr<FeedEvents> feed_events)
                                     {
                                       if (feed_events.ok())
                                       {
                                         buffer.push_back(feed_events.value());
                                       }
                                       else
                                       {
                                         // TODO if feed_evnets not okay
                                       }

                                       locker.unlock();
                                       cond.notify_one();
                                     });

    std::this_thread::sleep_for(std::chrono::seconds(1));
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
              { return buffer.size() > 0; });

    FeedEvents events = buffer.front();
    events_publisher.Publish(events);
    buffer.pop_front();

    locker.unlock();
    cond.notify_one();
    std::this_thread::sleep_for(std::chrono::seconds(1));
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