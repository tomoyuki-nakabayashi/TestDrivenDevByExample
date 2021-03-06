// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <utility>
#include <array>
#include <gtest/gtest.h>
#include <money.h>

namespace money_test{
using money::Money;
using money::Currency;
using money::Expression;
using money::Bank;
using money::Sum;

class MoneyTest : public ::testing::Test {
};

TEST_F(MoneyTest, Multiplication) {
  constexpr auto five = money::dollar(5);
  static_assert((five*2) == money::dollar(10), "product must be 10.");
  static_assert((five*3) == money::dollar(15), "product must be 15.");
}

TEST_F(MoneyTest, Equality) {
  static_assert(money::dollar(5) == money::dollar(5), "Two objects must be same.");
  static_assert(!(money::dollar(5) == money::dollar(6)),
    "Two objects must be different.");
  static_assert(!(money::franc(5) == money::dollar(5)),
    "Two objects must be different.");
}

TEST_F(MoneyTest, Currency) {
  static_assert(money::dollar(1).currency() == Currency::kUSD,
    "Dollar must have USD currency.");
  static_assert(money::franc(1).currency() == Currency::kCHF,
    "Franc must have CHF currency.");
}

TEST_F(MoneyTest, SimpleAddition) {
  constexpr Money five = money::dollar(5);
  constexpr auto sum = five + five;
  constexpr Bank bank;
  constexpr Money reduced = bank.reduce(sum, Currency::kUSD);
  static_assert(reduced == money::dollar(10), "sum must be 10 USD.");
}

TEST_F(MoneyTest, PlusReturnsSum) {
  constexpr Money five = money::dollar(5);
  constexpr auto sum = five + five;
  static_assert(sum.augend() == five, "Augend must be five dollar.");
  static_assert(sum.addend() == five, "Addend must be five dollar.");
}

TEST_F(MoneyTest, ReduceSum) {
  constexpr auto sum = money::dollar(3) + money::dollar(4);
  constexpr Bank bank;
  constexpr Money result = bank.reduce(sum, Currency::kUSD);
  static_assert(result == money::dollar(7), "Result must be seven dollar.");
}

TEST_F(MoneyTest, ReduceMoney) {
  constexpr Bank bank;
  constexpr Money result = bank.reduce(money::dollar(1), Currency::kUSD);
  static_assert(result == money::dollar(1), "Result must be one dollar.");
}

TEST_F(MoneyTest, ReduceMoneyDifferentCurrency) {
  constexpr Bank bank;
  constexpr auto new_bank = bank.addRate(Currency::kCHF, Currency::kUSD, 2);
  constexpr Money result = new_bank.reduce(money::franc(2), Currency::kUSD);
  static_assert(result == money::dollar(1), "Two franc must be one dollar.");
}

TEST_F(MoneyTest, IdentityRate) {
  constexpr Bank bank;
  static_assert(bank.rate(Currency::kUSD, Currency::kUSD) == 1,
    "Must be one if two currecy are same.");
}

TEST_F(MoneyTest, MixedAddition) {
  constexpr auto five_bucks = money::dollar(5);
  constexpr auto ten_francs = money::franc(10);
  constexpr Bank empty_bank;
  constexpr auto bank = empty_bank.addRate(Currency::kCHF, Currency::kUSD, 2);
  constexpr Money result = bank.reduce(five_bucks + ten_francs, Currency::kUSD);
  static_assert(result == money::dollar(10), "5 USD + 10 CHF must be 10 USD.");
}

TEST_F(MoneyTest, SumPlusMoney) {
  constexpr auto five_bucks = money::dollar(5);
  constexpr auto ten_francs = money::franc(10);
  constexpr Bank empty_bank;
  constexpr auto bank = empty_bank.addRate(Currency::kCHF, Currency::kUSD, 2);
  constexpr auto sum = (five_bucks + ten_francs) + five_bucks;
  constexpr Money result = bank.reduce(sum, Currency::kUSD);
  static_assert(result == money::dollar(15), "5 USD + 10 CHF + 5 USD must be 15 USD.");
}

TEST_F(MoneyTest, SumTimes) {
  constexpr auto five_bucks = money::dollar(5);
  constexpr auto ten_francs = money::franc(10);
  constexpr Bank empty_bank;
  constexpr auto bank = empty_bank.addRate(Currency::kCHF, Currency::kUSD, 2);
  constexpr auto sum = (five_bucks + ten_francs) * 2;
  constexpr Money result = bank.reduce(sum, Currency::kUSD);
  static_assert(result == money::dollar(20), "(5 USD + 10 CHF) *2 must be 20 USD.");
}

template <int N>
constexpr static std::array<int, N> createTable() {
  std::array<int, N> a{};
  for (int i = 0; i < N; ++i) {
    a[i] = i;
  }
  return a;
}

TEST_F(MoneyTest, CreateTable) {
  constexpr auto t1 = createTable<1>();
  constexpr auto t2 = createTable<1>();
  static_assert(t1[0] == t2[0], "can build.");
}
}  // namespace money_test
