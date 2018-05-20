// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include <array>
#include <algorithm>

namespace money {

// Forward declarations.
class Money;
template <class T, class U>
class Sum;
class Bank;

enum class Currency {
  kUSD, kCHF, kNoCurrency
};
constexpr static uint32_t kNumTradingCurrency = 2;

template <class Derived>
class Expression {
 public:
    constexpr Expression() = default;
    constexpr Money reduce(const Bank& bank, const Currency to) const;
};

class Money : public Expression<Money> {
 public:
    constexpr Money(int32_t amount, Currency currency = Currency::kNoCurrency)
        : amount_{amount}, currency_{currency} {}

    // Implements Expression interface.
    constexpr Money reduce(const Bank& bank, const Currency to) const;

    constexpr friend bool operator==(const Money& lhs, const Money& rhs) {
      return (lhs.amount_ == rhs.amount_) && (lhs.currency_ == rhs.currency_);
    }

    // Accessors.
    constexpr Currency currency() const {
      return currency_;
    }
    constexpr int32_t amount() const{
      return amount_;
    }

 private:
    int32_t amount_;
    Currency currency_;
};

struct Rate {
    int32_t rate;

 public:
    constexpr Rate()
        : rate{0}, from{Currency::kNoCurrency}, to{Currency::kNoCurrency} {}
    constexpr Rate(const Currency f, const Currency t, const int32_t r)
        : rate{r}, from{f}, to{t} {}
    constexpr friend bool operator==(const Rate& lhs, const Rate& rhs) {
      return ((lhs.from == rhs.from) && (lhs.to == rhs.to));
    }

 private:
    Currency from;
    Currency to;
};

namespace internal {
// A currency is exchanged to the other (means, n-1) currencies.
constexpr static size_t CalcTradingRateEntry(const uint32_t n) {
  return n*(n-1);
}

constexpr static size_t kNumRateEntry
    = CalcTradingRateEntry(kNumTradingCurrency);

constexpr size_t FindExistingEntry(const std::array<Rate, kNumRateEntry>& rates,
                                   const Rate& target, const size_t filled_index) {
  size_t i = 0;
  for (; i < filled_index; ++i) {
    if (rates[i] == target)
      break;
  }
  return i;
}
}  // namespace internal

class Bank {
 public:
    constexpr Bank(): rates_{}, index_for_new_entry_{0} {}
    constexpr Bank(const std::array<Rate, internal::kNumRateEntry>& rates, const size_t filled)
        : rates_{rates}, index_for_new_entry_{filled} {}
 
    // Implements Expression interface.
    template <class T>
    constexpr Money reduce(const Expression<T>& source, const Currency to) const {
      return source.reduce(*this, to);
    }

    constexpr Bank addRate(const Currency from, const Currency to, const int32_t rate) const {
      std::array<Rate, internal::kNumRateEntry> new_rates{rates_};
      size_t index_for_new_entry = index_for_new_entry_;
      const Rate target(from, to, rate);

      size_t index = internal::FindExistingEntry(rates_, target, index_for_new_entry_);
      if (index == index_for_new_entry_) {
        new_rates[index] = target;
        index_for_new_entry++;
      } else {
        new_rates[index].rate = rate;
      }

      return Bank{new_rates, index_for_new_entry};
    }
    constexpr int32_t rate(const Currency from, const Currency to) const {
      if (from == to) return 1;
      return findRate(from, to);
    }

 private:
    constexpr int32_t findRate(const Currency from, const Currency to) const {
      Rate target(from, to, 1);
      for (size_t i = 0; i < index_for_new_entry_; ++i) {
        if (rates_[i] == target)
          return rates_[i].rate;
      }
      return 0;
    }

 private:
    std::array<Rate, internal::kNumRateEntry> rates_;
    size_t index_for_new_entry_;
};

template <class T, class U>
class Sum : public Expression<Sum<T, U>> {
 public:
    constexpr Sum(const T& augend, const U& addend)
        : augend_{augend}, addend_{addend} {}

    // Implements Expression interface.
    constexpr Money reduce(const Bank& bank, const Currency to) const {
      auto amount = augend_.reduce(bank, to).amount()
         + addend_.reduce(bank, to).amount();
      return Money{amount, to};
    }

    // Accessors.
    constexpr T augend() const { return augend_; }
    constexpr U addend() const { return addend_; }

 private:
    T augend_;
    U addend_;
};

template <class Derived>
constexpr Money Expression<Derived>::reduce(const Bank& bank, const Currency to)const {
  return static_cast<const Derived&>(*this).reduce(bank, to);
}

constexpr Money Money::reduce(const Bank& bank, const Currency to) const {
  int rate = bank.rate(currency_, to);
  return Money(amount_ / rate, to);
}

// Factory methods.
constexpr Money dollar(int32_t amount) {
  return Money{amount, Currency::kUSD};
}

constexpr Money franc(int32_t amount) {
  return Money{amount, Currency::kCHF};
}

// Operators.
template <class T, class U>
constexpr Sum<T, U> operator+(const Expression<T>& lhs, const Expression<U>& rhs) {
  return Sum<T, U>{static_cast<T const&>(lhs), static_cast<U const&>(rhs)};
}

template <class T>
constexpr T operator*(const Expression<T>& lhs, const int32_t multiplier) {
  return T(static_cast<T const&>(lhs).augend() * multiplier,
           static_cast<T const&>(lhs).addend() * multiplier);
}

constexpr Money operator*(const Money& lhs, const int32_t multiplier) {
  return Money{lhs.amount() * multiplier, lhs.currency()};
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
