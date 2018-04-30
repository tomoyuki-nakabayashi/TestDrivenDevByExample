// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_DOLLAR_H_
#define CPP_TEMPLATE_DOLLAR_H_

#include <cstdint>
#include <money.h>

namespace money {

class Dollar : public Money {
 public:
    constexpr Dollar(int32_t amount) : Money{amount} {}
};

}  // namespace money

#endif  // CPP_TEMPLATE_DOLLAR_H_
