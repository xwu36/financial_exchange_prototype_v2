#ifndef LIB_PRICE4_H_
#define LIB_PRICE4_H_

#include <string>

namespace fep::lib
{
  class Price4
  {
  public:
    Price4() = default;
    explicit Price4(long unscaled) : unscaled_(unscaled) {}

    // convert from string
    explicit Price4(const std::string &str);

    long unscaled() const { return unscaled_; }

    // convert to string
    std::string to_str() const;

    bool operator==(const Price4 &that) const
    {
      return this->unscaled() == that.unscaled();
    }
    bool operator!=(const Price4 &that) const
    {
      return this->unscaled() != that.unscaled();
    }
    bool operator<(const Price4 &that) const
    {
      return this->unscaled() < that.unscaled();
    }
    bool operator<=(const Price4 &that) const
    {
      return this->unscaled() <= that.unscaled();
    }
    bool operator>(const Price4 &that) const
    {
      return this->unscaled() > that.unscaled();
    }
    bool operator>=(const Price4 &that) const
    {
      return this->unscaled() >= that.unscaled();
    }

  private:
    long unscaled_;
  };
}

#endif
