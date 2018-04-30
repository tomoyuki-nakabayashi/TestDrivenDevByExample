// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>

namespace money {

class Money {
 public:
    constexpr Money(int32_t amount) :amount_{amount} {}

    template <class T, class U,
                typename std::enable_if<
                  std::is_base_of<Money, T>::value &&
                  std::is_base_of<Money, U>::value &&
                  std::is_same<T, U>::value
                >::type* = nullptr
              >
    constexpr friend bool operator==(const T& rhs, const U& lhs) {
      return rhs.amount_ == lhs.amount_;
    }

    template <class T, class U,
                typename std::enable_if<
                  !std::is_same<T, U>::value
                >::type* = nullptr
              >
    constexpr friend bool operator==(const T&, const U&) {
      return false;
    }

    template <class T>
    constexpr friend T operator*(const T& rhs, int32_t multiplier) {
      return T{rhs.amount_*multiplier};
    }

 protected:
    int32_t amount_;
};

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
