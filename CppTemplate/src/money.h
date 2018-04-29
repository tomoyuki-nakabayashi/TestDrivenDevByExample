// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>

namespace money {

class Money {
 public:
    constexpr Money(int32_t amount) :amount_{amount} {}
    friend constexpr bool operator==(const Money& rhs, const Money& lhs);

 protected:
    int32_t amount_;
};

constexpr bool operator==(const Money& rhs, const Money& lhs) {
  return rhs.amount_ == lhs.amount_;
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
