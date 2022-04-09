#include "lib/timestamp.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace fep::lib
{
    namespace
    {
        TEST(TimestampTest, Now)
        {
            const auto expected_now = std::chrono::system_clock::now();
            EXPECT_EQ(std::chrono::duration_cast<std::chrono::seconds>(
                          expected_now.time_since_epoch())
                          .count(),
                      now_in_secs());
        }
        
    } // namespace
} // namespace fep::lib