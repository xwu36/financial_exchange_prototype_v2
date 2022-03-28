#include "src/feed_event/feed_event.h"

#include "absl/strings/str_join.h"
#include "nlohmann/json.hpp"

namespace fep::src::feed_event
{

    using ::nlohmann::json;

    std::string OrderTradeEvent::to_str() const
    {
        json j;
        j["type"] = this->type;
        j["price"] = this->price.to_str();
        j["quantity"] = this->quantity;
        return j.dump();
    }

    std::string PriceEntityUpdateEvent::to_str() const
    {
        json j;
        j["action"] = this->get_action();
        j["price"] = this->price.to_str();
        j["quantity"] = this->quantity;
        return j.dump();
    }

    std::string DepthUpdateEvents::to_str() const
    {
        json j;
        json jtype;
        jtype["type"] = this->type;

        json jbid;
        jbid["bid"] = {};
        for (const auto &bid : this->bid_events)
        {
            jbid["bid"].push_back(bid->to_str());
        }

        json jask;
        jask["ask"] = {};
        for (const auto &ask : this->ask_events)
        {
            jask["ask"].push_back(ask->to_str());
        }
        j.push_back(jtype);
        j.push_back(jbid);
        j.push_back(jask);
        return j.dump();
    }

    std::string FeedEvents::to_str() const
    {
        std::vector<std::string> trade_results;
        for (const auto &result : this->order_trade_events)
        {
            trade_results.push_back(result.to_str());
        }
        std::string results = absl::StrJoin(trade_results.begin(), trade_results.end(), "/n");
        return absl::StrCat(results, "/n", this->depth_update_events.to_str());
    }

} // namespace fep::src::feed_event