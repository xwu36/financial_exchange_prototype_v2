#include "lib/price4.h"

#include <string>

namespace fep::lib
{

  constexpr int kScale4 = 10000;

  Price4::Price4(const std::string &str)
  {
    unscaled_ = 0;
    int t = kScale4;
    bool see_decimal_point = false;
    for (const char c : str)
    {
      if (c == '.')
      {
        see_decimal_point = true;
        continue;
      }
      if (see_decimal_point)
      {
        t /= 10;
      }
      if (t == 0)
      {
        return;
      }
      unscaled_ = unscaled_ * 10 + (c - '0');
    }
    unscaled_ *= t;
  }

  std::string Price4::to_str() const
  {
    long residue = unscaled_ - unscaled_ / kScale4 * kScale4;
    const std::string int_part = std::to_string(unscaled_ / kScale4);

    std::string fraction = ".0000";
    int i = 4;
    while (residue > 0 && i >= 0)
    {
      int d = residue % 10;
      fraction[i] = d + '0';
      residue /= 10;
      --i;
    }

    if (fraction[3] == '0' && fraction[4] == '0')
    {
      return int_part + fraction.substr(0, 3);
    }

    if (fraction[4] == '0')
    {
      return int_part + fraction.substr(0, 4);
    }

    return int_part + fraction;
  }

} // namespace fep::lib