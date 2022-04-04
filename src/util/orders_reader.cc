#include "src/util/orders_reader.h"

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
#include "src/order/order_parser.h"

namespace fep::src::util
{
    using ::fep::src::order::Order;
    using ::nlohmann::json;

    std::vector<Order> ReadOrdersFromPath(const std::string &path)
    {
        std::string line;
        std::ifstream infile(path);
        std::vector<Order> orders;
        while (std::getline(infile, line))
        {
            json j = json::parse(line);
            orders.emplace_back(j.get<Order>());
        }
        return orders;
    }

} // namespace fep::src::util