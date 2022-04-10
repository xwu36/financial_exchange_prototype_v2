#ifndef LIB_TIMESTAMP_NOW_H_
#define LIB_TIMESTAMP_NOW_H_

#include <chrono>

namespace fep::lib
{
  int64_t now_in_secs()
  {
    const auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(
               now.time_since_epoch())
        .count();
  }
}

#endif
