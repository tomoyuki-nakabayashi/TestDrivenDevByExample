// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>

namespace money {

class Dollar {
 public:
    constexpr Dollar(int32_t amount) :amount_{amount} {}

 public:
    int amount_;
};

}  // namespace money

constexpr money::Dollar operator*(const money::Dollar& rhs, const money::Dollar& lhs) {
  return money::Dollar{rhs.amount_*lhs.amount_};
}

constexpr bool operator==(const money::Dollar& rhs, const money::Dollar& lhs) {
  return rhs.amount_ == lhs.amount_;
}

#endif  // CPP_TEMPLATE_MONEY_H_
