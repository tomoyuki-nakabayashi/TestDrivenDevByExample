// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <gtest/gtest.h>
#include <dollar.h>
#include <franc.h>

namespace money_test{
using money::Dollar;
using money::Franc;

class MoneyTest : public ::testing::Test {
};

TEST_F(MoneyTest, Multiplication) {
  constexpr Dollar five(5);
  static_assert((five*2) == Dollar{10}, "product must be 10.");
  static_assert((five*3) == Dollar{15}, "product must be 15.");
}

TEST_F(MoneyTest, Equality) {
  static_assert(Dollar{5} == Dollar{5}, "Two objects must be same.");
  static_assert(!(Dollar{5} == Dollar{6}), "Two objects must be different.");
  static_assert(Franc{5} == Franc{5}, "Two objects must be same.");
  static_assert(!(Franc{5} == Franc{6}), "Two objects must be different.");
}

TEST_F(MoneyTest, FrancMultiplication) {
  constexpr Franc five(5);
  static_assert((five*2) == Franc{10}, "product must be 10.");
  static_assert((five*3) == Franc{15}, "product must be 15.");
}

}  // namespace money_test
