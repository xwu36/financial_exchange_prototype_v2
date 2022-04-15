#include "lib/timestamp.h"

namespace fep::lib
{
    int64_t now_in_secs()
    {
        const auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(
                   now.time_since_epoch())
            .count();
    };

} // namespace fep::lib