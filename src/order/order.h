#ifndef SRC_ORDER_ORDER_H_
#define SRC_ORDER_ORDER_H_

#include <cstdint>
#include <string>

#include "lib/price4.h"
#include "nlohmann/json.hpp"
#include "src/stock/symbol.h"

namespace fep::src::order
{
  constexpr char kTime[] = "time";
  constexpr char kTimestampSec[] = "timestamp_sec";
  constexpr char kOrderId[] = "order_id";
  constexpr char kQuantity[] = "quantity";
  constexpr char kLimitPrice[] = "limit_price";
  constexpr char kSymbol[] = "symbol";
  constexpr char kType[] = "type";
  constexpr char kSide[] = "side";
  constexpr char kOrderType[] = "order_type";
  constexpr char kTimeInForce[] = "time_in_force";
  constexpr char kNew[] = "NEW";
  constexpr char kCancel[] = "CANCEL";
  constexpr char kBuy[] = "BUY";
  constexpr char kSell[] = "SELL";
  constexpr char kMarket[] = "MARKET";
  constexpr char kLimit[] = "LIMIT";
  constexpr char kIceberg[] = "ICEBERG";
  constexpr char kDay[] = "DAY";
  constexpr char kIoc[] = "IOC";
  constexpr char kGtc[] = "GTC";

  enum class OrderType
  {
    UNKNOWN,
    MARKET,
    LIMIT,
    ICEBERG,
  };

  enum class OrderStatus
  {
    UNKNOWN,
    NEW,
    CANCEL
  };

  enum class OrderSide
  {
    UNKNOWN,
    BUY,
    SELL
  };

  enum class TimeInForce
  {
    UNKNOWN,
    // A DAY order can rest in the order book if not immediately traded.
    DAY,
    // Immediate-or-cancel. An IOC order cannot rest in the order book.
    IOC,
    // Good-till-cancel. A GTC order is like a DAY order with one distinction.
    GTC
  };

  struct Order
  {
    int64_t timestamp_sec = 0;
    OrderStatus type = OrderStatus::UNKNOWN;
    int64_t order_id = 0;
    fep::src::stock::Symbol symbol = fep::src::stock::Symbol::UNKNOWN;
    OrderSide side = OrderSide::UNKNOWN;
    int32_t quantity = 0;
    fep::lib::Price4 price;
    OrderType order_type = OrderType::UNKNOWN;
    TimeInForce time_in_force = TimeInForce::UNKNOWN;
  };

} // namespace fep::src::order

#endif