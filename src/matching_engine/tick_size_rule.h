#ifndef SRC_MATCHING_ENGINE_TICK_SIZE_RULE_H_
#define SRC_MATCHING_ENGINE_TICK_SIZE_RULE_H_

#include <vector>

#include "lib/price4.h"
#include "nlohmann/json.hpp"

namespace fep::src::matching_engine
{
  struct Tick
  {
    fep::lib::Price4 from_price;
    fep::lib::Price4 to_price;
    fep::lib::Price4 tick_size;
  };

  class TickSizeRule
  {
  public:
    TickSizeRule() = default;
    bool FromJson(const nlohmann::json &data);
    const std::vector<Tick> &GetTicks() const
    {
      return ticks;
    }

  private:
    std::vector<Tick> ticks;
  };

} // namespace fep::src::matching_engine

#endif