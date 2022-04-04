
#ifndef SRC_UTIL_ORDERS_READER_H_
#define SRC_UTIL_ORDERS_READER_H_

#include <string>
#include <vector>

#include "src/order/order.h"

namespace fep::src::util
{

    std::vector<fep::src::order::Order> ReadOrdersFromPath(const std::string &path);

} // namespace fep::src::util

#endif