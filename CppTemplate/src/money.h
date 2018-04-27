// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>

namespace money {

class Dollar {
 public:
    constexpr Dollar(int32_t amount) :amount_{amount} {}

    constexpr Dollar times(int32_t multiplier) const {
        return Dollar(amount_*multiplier);
    }

 public:
    int amount_;
};

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
