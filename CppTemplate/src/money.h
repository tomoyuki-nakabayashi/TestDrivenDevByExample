// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>

namespace money {

class Sum;
class Money;
class Bank;

enum class Currency {
  kUSD, kCHF, kNoCurrency
};

template <class Derived>
class Expression {
 public:
    constexpr Expression() {}
    constexpr Money reduce(const Bank& bank, const Currency to) const;
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

    constexpr Money reduce(const Bank& bank, const Currency to) const;

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

class Bank {
 public:
    constexpr Bank() {}
    template <class T>
    constexpr Money reduce(const Expression<T>& source, const Currency to) const {
      return source.reduce(*this, to);
    }
    constexpr void addRate(const Currency from, const Currency to, int32_t rate) const {

    }
    constexpr int32_t rate(const Currency from, const Currency to) const {
      return ((from == Currency::kCHF) && (to == Currency::kUSD)) ? 2 : 1;
    }
};

template <class Derived>
constexpr Money Expression<Derived>::reduce(const Bank& bank, const Currency to)const {
  return static_cast<const Derived&>(*this).reduce(bank, to);
}

class Sum : public Expression<Sum> {
 public:
    constexpr Sum(const Money& augend, const Money& addend)
        : augend_{augend}, addend_{addend} {}
    constexpr Money reduce(const Bank& bank, const Currency to) const {
      auto amount = augend_.amount() + addend_.amount();
      return Money{amount, to};
    }

 public:
    Money augend_;
    Money addend_;
};

constexpr Money Money::reduce(const Bank& bank, const Currency to) const {
  int rate = bank.rate(currency_, to);
  return Money(amount_ / rate, to);
}

constexpr Money dollar(int32_t amount) {
  return Money{amount, Currency::kUSD};
}

constexpr Money franc(int32_t amount) {
  return Money{amount, Currency::kCHF};
}

constexpr Sum operator+(const Money& rhs, const Money& lhs) {
  return Sum{rhs, lhs};
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
