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
  constexpr auto five = money::factory<Dollar>(5);
  static_assert((five*2) == money::factory<Dollar>(10), "product must be 10.");
  static_assert((five*3) == money::factory<Dollar>(15), "product must be 15.");
}

TEST_F(MoneyTest, Equality) {
  static_assert(money::factory<Dollar>(5) == money::factory<Dollar>(5), "Two objects must be same.");
  static_assert(!(money::factory<Dollar>(5) == money::factory<Dollar>(6)), "Two objects must be different.");
  static_assert(money::factory<Franc>(5) == money::factory<Franc>(5), "Two objects must be same.");
  static_assert(!(money::factory<Franc>(5) == money::factory<Franc>(6)), "Two objects must be different.");
  static_assert(!(money::factory<Franc>(5) == money::factory<Dollar>(5)), "Two objects must be different.");
}

TEST_F(MoneyTest, FrancMultiplication) {
  constexpr auto five = money::factory<Franc>(5);
  static_assert((five*2) == money::factory<Franc>(10), "product must be 10.");
  static_assert((five*3) == money::factory<Franc>(15), "product must be 15.");
}

TEST_F(MoneyTest, Currency) {
  static_assert(money::factory<Dollar>(1).currency() == Currency::kUSD, "Dollar must have USD currency.");
  static_assert(money::factory<Franc>(1).currency() == Currency::kCHF, "Franc must have CHF currency.");
}

}  // namespace money_test
