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

template <class Derived>
class Money {
 public:
    constexpr Money(int32_t amount, Currency currency = Currency::kNoCurrency)
        : amount_{amount}, currency_{currency} {}
    constexpr Currency currency() const {
      return Derived{0}.currency_;
    }

    template <class T,
                typename std::enable_if<
                  std::is_same<Derived, T>::value
                >::type* = nullptr
              >
    constexpr friend bool operator==(const Money<Derived>& rhs, const Money<T>& lhs) {
      return (rhs.amount_ == lhs.amount_) && (Derived{0}.currency_ == T{0}.currency_);
    }

    template <class T,
                typename std::enable_if<
                  !std::is_same<Derived, T>::value
                >::type* = nullptr
              >
    constexpr friend bool operator==(const Money<Derived>&, const Money<T>&) {
      return false;
    }

    constexpr friend Money<Derived> operator*(const Money<Derived>& rhs, int32_t multiplier) {
      return Money<Derived>{rhs.amount_*multiplier};
    }

 protected:
    int32_t amount_;
    Currency currency_;
};

class Dollar : public Money<Dollar> {
 public:
    constexpr Dollar(int32_t amount = 0)
        : Money{amount, Currency::kUSD} {}
};

class Franc : public Money<Franc> {
 public:
    constexpr Franc(int32_t amount = 0)
        : Money{amount, Currency::kCHF} {}
};

template <class T>
constexpr Money<T> factory(int32_t amount) {
  return Money<T>{amount, Currency::kNoCurrency};
}

template <>
constexpr Money<Dollar> factory(int32_t amount) {
  return Money<Dollar>{amount, Currency::kUSD};
}

template <>
constexpr Money<Franc> factory(int32_t amount) {
  return Money<Franc>{amount, Currency::kCHF};
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
