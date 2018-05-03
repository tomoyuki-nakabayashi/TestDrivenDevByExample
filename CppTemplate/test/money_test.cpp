// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <gtest/gtest.h>
#include <dollar.h>
#include <franc.h>

namespace money_test{
using money::Money;
using money::Currency;

class MoneyTest : public ::testing::Test {
};

TEST_F(MoneyTest, Multiplication) {
  constexpr auto five = money::dollar(5);
  static_assert((five*2) == money::dollar(10), "product must be 10.");
  static_assert((five*3) == money::dollar(15), "product must be 15.");
}

TEST_F(MoneyTest, Equality) {
  static_assert(money::dollar(5) == money::dollar(5), "Two objects must be same.");
  static_assert(!(money::dollar(5) == money::dollar(6)), "Two objects must be different.");
  static_assert(!(money::franc(5) == money::dollar(5)), "Two objects must be different.");
}

TEST_F(MoneyTest, Currency) {
  static_assert(money::dollar(1).currency() == Currency::kUSD, "Dollar must have USD currency.");
  static_assert(money::franc(1).currency() == Currency::kCHF, "Franc must have CHF currency.");
}

}  // namespace money_test
