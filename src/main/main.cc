#include <condition_variable>
#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/strings/str_cat.h"
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

ABSL_FLAG(std::string, path_prefix, "srcs/main/data/", "Directory which holds the order files");

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

void RunMatchingEngine()
{
  absl::TimeZone timezone;
  absl::LoadTimeZone(kNewYorkTimeZone, &timezone);

  const std::string path_prefix = absl::GetFlag(FLAGS_path_prefix);
  const std::string date_now = absl::FormatTime("%Y-%m-%d/", absl::Now(), timezone);
  const std::string date_a_day_ago = absl::FormatTime("%Y-%m-%d/", absl::Now() - absl::Hours(24), timezone);
  const std::string orders_to_process = absl::StrCat(path_prefix, date_now, "orders.jsonl");
  const std::string unmatched_orders_a_day_ago = absl::StrCat(path_prefix, date_a_day_ago, "unmatched_orders.jsonl");
  const std::string unmatched_orders_today = absl::StrCat(path_prefix, date_now, "unmatched_orders.jsonl");
  LOG(INFO) << "The path of all offers to be processed today: " << orders_to_process;
  LOG(INFO) << "The path of unmatched GTC offers from yesterday: " << unmatched_orders_a_day_ago;
  LOG(INFO) << "The path of unmatched GTC offers for today: " << unmatched_orders_today;

  MatchingEngine matching_engine;

  // Read yesterday's unmatched GTC orders.
  absl::Status init_stats = matching_engine.InitOnMarketStarts(unmatched_orders_a_day_ago);
  if (!init_stats.ok())
  {
    LOG(ERROR) << init_stats.message();
  }

  // Process today's orders.
  std::vector<Order> orders = fep::src::util::ReadOrdersFromPath("src/main/data/orders.jsonl");
  // std::vector<Order> orders = fep::src::util::ReadOrdersFromPath(orders_to_process);
  // Loop through all the offers and process each of them.
  for (const Order &order : orders)
  {
    LOG(INFO) << "Receive new order at " << fep::lib::now_in_secs();

    absl::Time order_time = absl::FromUnixSeconds(order.timestamp_sec);
    const std::string order_time_str = absl::FormatTime("%H:%M", order_time, timezone);
    if (order_time_str > "16:00")
    {
      LOG(INFO) << "Market is closed!";
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
                                         LOG(WARNING) << "order cannot be successfully processed.";
                                         // TODO if feed_evnets not okay
                                       }

                                       locker.unlock();
                                       g_feed_cond.notify_one();
                                     });

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // Clear the matching engine and output today's unmatched GTC orders.
  absl::Status clear_status = matching_engine.ClearOnMarketEnds(unmatched_orders_today);
  if (!clear_status.ok())
  {
    LOG(ERROR) << clear_status.message();
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

void Run()
{
  std::thread t1(RunMatchingEngine);
  std::thread t2(RunPublisher);

  t1.join();
  t2.join();
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

int main()
{
  std::thread market_status_thread(MarketStatusChecker);
  std::thread match_publish_thread(MatchAndPublish);

  market_status_thread.join();
  match_publish_thread.join();

  return 0;
}