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

    Price4(const Price4& price) : Price4(price.unscaled_) {}

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

    // The following is for test purpose only.
    int a_ = 0;
    std::string b_;

  private:
    long unscaled_;
  };
}

namespace std
{
  template <>
  struct hash<fep::lib::Price4>
  {
    std::size_t operator()(const fep::lib::Price4 &k) const
    {
      return std::hash<long>()(k.unscaled());
    }
  };
}

#endif
