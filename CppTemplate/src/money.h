// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>

namespace money {

enum class Currency {
  kUSD, kCHF, kNoCurrency
};

class Money {
 public:
    constexpr Money(int32_t amount, Currency currency)
        : amount_{amount}, currency_{currency} {}
    constexpr Currency currency() const {
      return currency_;
    }

    constexpr friend bool operator==(const Money& rhs, const Money& lhs) {
      return (rhs.amount_ == lhs.amount_) && (rhs.currency_ == lhs.currency_);
    }

    constexpr friend Money operator*(const Money rhs, int32_t multiplier) {
      return Money{rhs.amount_*multiplier, rhs.currency_};
    }

 protected:
    int32_t amount_;
    Currency currency_;
};

constexpr Money dollar(int32_t amount) {
  return Money{amount, Currency::kUSD};
}

constexpr Money franc(int32_t amount) {
  return Money{amount, Currency::kCHF};
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
