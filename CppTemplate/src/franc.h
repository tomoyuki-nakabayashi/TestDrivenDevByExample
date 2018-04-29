// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_FRANC_H_
#define CPP_TEMPLATE_FRANC_H_

#include <cstdint>

namespace money {

class Franc {
 public:
    constexpr Franc(int32_t amount) :amount_{amount} {}
    constexpr bool operator==(const Franc& lhs) const {
      return amount_ == lhs.amount_;
    }
    friend constexpr Franc operator*(const Franc& rhs, const Franc& lhs);

 private:
    int amount_;
};

constexpr Franc operator*(const Franc& rhs, const Franc& lhs) {
  return Franc{rhs.amount_*lhs.amount_};
}

}  // namespace money

#endif  // CPP_TEMPLATE_FRANC_H_
