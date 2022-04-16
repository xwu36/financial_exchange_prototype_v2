#include <condition_variable>
#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>

#include "absl/time/time.h"
#include "absl/time/clock.h"
#include "external/com_google_absl/absl/status/statusor.h"
#include "glog/logging.h"
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

std::mutex g_feed_mu;
std::condition_variable g_feed_cond;
std::deque<FeedEvents> g_feed_buffer;
const unsigned int maxBufferSize = 2;

std::mutex g_market_mu;
bool g_market_starts = false;
std::condition_variable g_market_cond;

constexpr char kNewYorkTimeZone[] = "America/New_York";

void MarketStatusChecker()
{
  absl::TimeZone timezone;
  absl::LoadTimeZone(kNewYorkTimeZone, &timezone);
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(60));

    const std::string now_str = absl::FormatTime("%H:%M", absl::Now(), timezone);
    LOG(INFO) << "Checking market status at " << now_str << "...";
    std::unique_lock<std::mutex> locker(g_market_mu);
    g_market_starts = (now_str == "09:00");
    locker.unlock();
    g_market_cond.notify_one();
  }
}

void MatchAndPublish()
{
  while (true)
  {
    std::unique_lock<std::mutex> locker(g_market_mu);
    g_market_cond.wait(locker, []()
                       { return g_market_starts; });
    LOG(INFO) << "Market starts!";
    g_market_starts = false;
    locker.unlock();
    Run();
  }
}

void Run()
{
  std::thread t1(RunMatchingEngine);
  std::thread t2(RunPublisher);

  t1.join();
  t2.join();
}

void RunMatchingEngine()
{
  absl::TimeZone timezone;
  absl::LoadTimeZone(kNewYorkTimeZone, &timezone);

  MatchingEngine matching_engine;
  std::vector<Order> orders = fep::src::util::ReadOrdersFromPath("src/main/data/orders.jsonl");
  // Loop through all the offers and process each of them.
  for (const Order &order : orders)
  {
    LOG(INFO) << "Receive new order at " << fep::lib::now_in_secs();

    absl::Time order_time = absl::FromUnixSeconds(order.timestamp_sec);
    const std::string order_time_str = absl::FormatTime("%H:%M", order_time, timezone);
    if (order_time_str > "16:00")
    {
      return;
    }

    std::unique_lock<std::mutex> locker(g_feed_mu);
    // g_feed_cond.wait(locker, []
    //           { return g_feed_buffer.size() < maxBufferSize; });

    matching_engine.ProcessAndNotify(std::make_shared<Order>(order),
                                     [&](absl::StatusOr<FeedEvents> feed_events)
                                     {
                                       if (feed_events.ok())
                                       {
                                         g_feed_buffer.push_back(feed_events.value());
                                       }
                                       else
                                       {
                                         // TODO if feed_evnets not okay
                                       }

                                       locker.unlock();
                                       g_feed_cond.notify_one();
                                     });

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void RunPublisher()
{
  EventsPublisher events_publisher;
  while (true)
  {
    std::unique_lock<std::mutex> locker(g_feed_mu);
    g_feed_cond.wait(locker, []()
                     { return g_feed_buffer.size() > 0; });

    FeedEvents events = g_feed_buffer.front();
    events_publisher.Publish(events);
    g_feed_buffer.pop_front();

    locker.unlock();
    // g_feed_cond.notify_one();

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main()
{
  std::thread market_status_thread(MarketStatusChecker);
  std::thread match_publish_thread(MatchAndPublish);

  market_status_thread.join();
  match_publish_thread.join();

  return 0;
}