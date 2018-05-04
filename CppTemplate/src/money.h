// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include <expression.h>

namespace money {

class Sum;

enum class Currency {
  kUSD, kCHF, kNoCurrency
};

class Money : public Expression<Money> {
 public:
    constexpr Money(int32_t amount, Currency currency = Currency::kNoCurrency)
        : amount_{amount}, currency_{currency} {}
    constexpr Currency currency() const {
      return currency_;
    }
    constexpr int32_t amount() const{
      return amount_;
    }

    constexpr friend bool operator==(const Money& rhs, const Money& lhs) {
      return (rhs.amount_ == lhs.amount_) && (rhs.currency_ == lhs.currency_);
    }

    constexpr friend Sum operator+(const Money& rhs, const Money& lhs);

    constexpr friend Money operator*(const Money& rhs, int32_t multiplier) {
      return Money{rhs.amount_*multiplier, rhs.currency_};
    }

 private:
    int32_t amount_;
    Currency currency_;
};

constexpr Money dollar(int32_t amount) {
  return Money{amount, Currency::kUSD};
}

constexpr Money franc(int32_t amount) {
  return Money{amount, Currency::kCHF};
}


class Sum : public Expression<Sum> {
 public:
    constexpr Sum(const Money& augend, const Money& addend)
        : augend_{augend}, addend_{addend} {}
    constexpr Money reduce(const Currency& to) const {
      auto amount = augend_.amount() + addend_.amount();
      return Money{amount, to};
    }

 public:
    Money augend_;
    Money addend_;
};

constexpr Sum operator+(const Money& rhs, const Money& lhs) {
  return Sum{rhs, lhs};
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
