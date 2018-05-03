// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_BANK_H_
#define CPP_TEMPLATE_BANK_H_

#include <cstdint>
#include <money.h>
#include <expression.h>

namespace money {

class Bank {
 public:
    constexpr Bank() {}

    template <class T>
    constexpr Money reduce (Expression<T> source, Currency to) const {
      return dollar(10);
    }
};

}  // namespace money

#endif  // CPP_TEMPLATE_BANK_H_
