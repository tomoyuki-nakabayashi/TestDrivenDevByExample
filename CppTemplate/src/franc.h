// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_FRANC_H_
#define CPP_TEMPLATE_FRANC_H_

#include <cstdint>
#include <money.h>

namespace money {

class Franc : public Money<Franc> {
 public:
    constexpr Franc(int32_t amount) : Money{amount} {}
    constexpr Currency currency() const {
      return Currency::kCHF;
    }
};

}  // namespace money

#endif  // CPP_TEMPLATE_FRANC_H_
