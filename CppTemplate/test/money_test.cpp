// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <gtest/gtest.h>
#include <dollar.h>
#include <franc.h>

namespace money_test{
using money::Money;
using money::Currency;
using money::Dollar;
using money::Franc;

class MoneyTest : public ::testing::Test {
};

TEST_F(MoneyTest, Multiplication) {
  constexpr Money<Dollar> five{5};
  static_assert((five*2) == Money<Dollar>{10}, "product must be 10.");
  static_assert((five*3) == Money<Dollar>{15}, "product must be 15.");
}

TEST_F(MoneyTest, Equality) {
  static_assert(Money<Dollar>{5} == Money<Dollar>{5}, "Two objects must be same.");
  static_assert(!(Money<Dollar>{5} == Money<Dollar>{6}), "Two objects must be different.");
  static_assert(Money<Franc>{5} == Money<Franc>{5}, "Two objects must be same.");
  static_assert(!(Money<Franc>{5} == Money<Franc>{6}), "Two objects must be different.");
  static_assert(!(Money<Franc>{5} == Money<Dollar>{5}), "Two objects must be different.");
}

TEST_F(MoneyTest, FrancMultiplication) {
  constexpr Money<Franc> five{5};
  static_assert((five*2) == Money<Franc>{10}, "product must be 10.");
  static_assert((five*3) == Money<Franc>{15}, "product must be 15.");
}

TEST_F(MoneyTest, Currency) {
  static_assert(Money<Dollar>{1}.currency() == Currency::kUSD, "Dollar must have USD currency.");
  static_assert(Money<Franc>{1}.currency() == Currency::kCHF, "Franc must have CHF currency.");
}

}  // namespace money_test
