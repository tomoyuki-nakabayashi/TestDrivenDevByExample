// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>

namespace money {

enum class Currency {
  kUSD, kCHF
};

template <class Derived>
class Money {
 public:
    constexpr Money(int32_t amount, Currency currency = Currency::kUSD)
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
      return rhs.amount_ == lhs.amount_;
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

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
