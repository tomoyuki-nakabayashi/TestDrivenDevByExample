// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>

namespace money {

struct money_type {};

class is_money_impl {
 private:
    template <class T>
    static constexpr std::true_type check(typename T::money_type*);

    template <class T>
    static constexpr std::false_type check(...);
};

template <class T>
struct is_money : decltype(is_money_impl::check<T>(nullptr)) {};

class Money {
 public:
    constexpr Money(int32_t amount) :amount_{amount} {}
    //template <class T, class U>
    //friend constexpr bool operator==(const T& rhs, const U& lhs);
    //money_type value_type;

 public:
    int32_t amount_;
};

template <class T, class U,
          typename std::enable_if<std::is_same<T, U>::value>::type* = nullptr>
constexpr bool operator==(const T& rhs, const U& lhs) {
  return rhs.amount_ == lhs.amount_;
}

template <class T, class U,
          typename std::enable_if<!std::is_same<T, U>::value>::type* = nullptr>
constexpr bool operator==(const T& /*rhs*/, const U& /*lhs*/) {
  return false;
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
