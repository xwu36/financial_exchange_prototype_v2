#ifndef SRC_UTIL_JSON_PARSER_H_
#define SRC_UTIL_JSON_PARSER_H_

#include <string>

#include "absl/strings/string_view.h"
#include "lib/price4.h"
#include "nlohmann/json.hpp"
#include "src/order/order.h"
#include "src/stock/symbol.h"

// from_json methods
namespace fep::src::order
{
  template <class T>
  T GetValueForKey(const nlohmann::json &j, absl::string_view key, const T &default_value)
  {
    T res = default_value;
    const auto itr = j.find(std::string(key));
    if (itr == j.end())
    {
      return res;
    }
    return itr->get<T>();
  }

  static void from_json(const nlohmann::json &j, fep::src::order::Order &order)
  {
    order.timestamp_sec = GetValueForKey<int64_t>(j, kTime, /*default_value=*/0);
    order.order_id = GetValueForKey<int64_t>(j, kOrderId, /*default_value=*/0);
    order.quantity = GetValueForKey<int32_t>(j, kQuantity, /*default_value=*/0);
    order.hidden_quantity = GetValueForKey<int32_t>(j, kHiddenQuantity, /*default_value=*/0);
    order.price = fep::lib::Price4(GetValueForKey<std::string>(j, kLimitPrice, /*default_value=*/"0"));

    const std::string type = GetValueForKey<std::string>(j, kType, /*default_value=*/"");
    if (type == "NEW")
    {
      order.type = OrderStatus::NEW;
    }
    else if (type == "CANCEL")
    {
      order.type = OrderStatus::CANCEL;
    }

    const std::string symbol = GetValueForKey<std::string>(j, kSymbol, /*default_value=*/"");
    const auto symbol_enum = fep::src::stock::SymbolFromString.find(symbol);
    if (symbol_enum != fep::src::stock::SymbolFromString.end())
    {
      order.symbol = symbol_enum->second;
    }

    const std::string side = GetValueForKey<std::string>(j, kSide, /*default_value=*/"");
    if (side == kBuy)
    {
      order.side = OrderSide::BUY;
    }
    else if (side == kSell)
    {
      order.side = OrderSide::SELL;
    }

    const std::string order_type = GetValueForKey<std::string>(j, kOrderType, /*default_value=*/"");
    if (order_type == kLimit)
    {
      order.order_type = OrderType::LIMIT;
    }
    else if (order_type == kMarket)
    {
      order.order_type = OrderType::MARKET;
    }
    else if (order_type == kIceberg)
    {
      // TODO: Add unit tests.
      order.order_type = OrderType::ICEBERG;
      order.display_quantity = order.quantity;
    }

    const std::string time_in_force = GetValueForKey<std::string>(j, kTimeInForce, /*default_value=*/"");
    if (time_in_force == kDay)
    {
      order.time_in_force = TimeInForce::DAY;
    }
    else if (time_in_force == kIoc || order.order_type == OrderType::ICEBERG)
    {
      order.time_in_force = TimeInForce::IOC;
    }
    else if (time_in_force == kGtc)
    {
      order.time_in_force = TimeInForce::GTC;
    }
  }

  static void to_json(nlohmann::json &j, fep::src::order::Order &order)
  {
    j[kTimestampSec] = order.timestamp_sec;

    switch (order.type)
    {
    case OrderStatus::NEW:
      j[kType] = kNew;
      break;
    case OrderStatus::CANCEL:
      j[kType] = kCancel;
      break;
    default:
      break;
    }

    j[kOrderId] = order.order_id;
    const auto symbol = fep::src::stock::SymbolToString.find(order.symbol);
    if (symbol != fep::src::stock::SymbolToString.end())
    {
      j[kSymbol] = symbol->second;
    }

    switch (order.side)
    {
    case OrderSide::BUY:
      j[kSide] = kBuy;
      break;
    case OrderSide::SELL:
      j[kSide] = kSell;
      break;
    default:
      break;
    }

    j[kQuantity] = order.quantity;
    j[kLimitPrice] = order.price.to_str();
    j[kDisplayQuantity] = order.display_quantity;

    switch (order.order_type)
    {
    case OrderType::MARKET:
      j[kOrderType] = kMarket;
      break;
    case OrderType::LIMIT:
      j[kOrderType] = kLimit;
      break;
    case OrderType::ICEBERG:
      j[kOrderType] = kIceberg;
    default:
      break;
    }

    switch (order.time_in_force)
    {
    case TimeInForce::DAY:
      j[kTimeInForce] = kDay;
      break;
    case TimeInForce::IOC:
      j[kTimeInForce] = kIoc;
      break;
    case TimeInForce::GTC:
      j[kTimeInForce] = kGtc;
      break;
    default:
      break;
    }
  }

} // fep::src::order

#endif