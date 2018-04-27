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
  five.times();
  static_assert(five.amount_ == 10, "five must be 10");
} */

}  // namespace money_test
