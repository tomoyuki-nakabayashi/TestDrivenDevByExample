// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#include <gtest/gtest.h>
#include <money.h>

namespace money_test{
using money::Dollar;

class MoneyTest : public ::testing::Test {
};

TEST_F(MoneyTest, Multiplication) {
  constexpr Dollar five(5);
  constexpr auto product_by_two = five * 2;
  static_assert(product_by_two.amount_ == 10, "product must be 10.");
  constexpr auto product_by_three = 3 * five;
  static_assert(product_by_three.amount_ == 15, "product must be 15.");
}

}  // namespace money_test
