// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_FRANC_H_
#define CPP_TEMPLATE_FRANC_H_

#include <cstdint>
#include <money.h>

namespace money {

class Franc : public Money {
 public:
    constexpr Franc(int32_t amount) : Money{amount} {}

    friend constexpr Franc operator*(const Franc& rhs, const Franc& lhs);
};

constexpr Franc operator*(const Franc& rhs, const Franc& lhs) {
  return Franc{rhs.amount_*lhs.amount_};
}

}  // namespace money

#endif  // CPP_TEMPLATE_FRANC_H_
