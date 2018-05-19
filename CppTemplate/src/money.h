// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include <array>

namespace money {

// Forward declarations.
class Money;
template <class T, class U>
class Sum;
template <int32_t N>
class Bank;

enum class Currency {
  kUSD, kCHF, kNoCurrency
};

template <class Derived>
class Expression {
 public:
    constexpr Expression() = default;
    template <int32_t N>
    constexpr Money reduce(const Bank<N>& bank, const Currency to) const;
};

class Money : public Expression<Money> {
 public:
    constexpr Money(int32_t amount, Currency currency = Currency::kNoCurrency)
        : amount_{amount}, currency_{currency} {}

    // Implements Expression interface.
    template <int32_t N>
    constexpr Money reduce(const Bank<N>& bank, const Currency to) const;

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

class TransrationRate {
 public:
    constexpr TransrationRate()
        : from_{Currency::kNoCurrency}, to_{Currency::kNoCurrency}, rate_{0} {}
    constexpr TransrationRate(const Currency from, const Currency to, const int32_t rate)
        : from_{from}, to_{to}, rate_{rate} {}
    constexpr TransrationRate(const TransrationRate& other) = default;
    TransrationRate& operator=(const TransrationRate& other) = default;
    bool operator==(const TransrationRate& rhs) {
      return ((from_ == rhs.from_) && (to_ == rhs.to_));
    }

 private:
    Currency from_;
    Currency to_;
    int32_t rate_;
};

using Hash = std::pair<Currency, Currency>;
using Rate = std::pair<Hash, int>;
template <int32_t N>
class Bank {
 public:
    constexpr Bank(const std::array<Rate, N>& rates) : rates_{rates} {}
 
    // Implements Expression interface.
    template <class T>
    constexpr Money reduce(const Expression<T>& source, const Currency to) const {
      return source.reduce(*this, to);
    }

    constexpr Bank<N+1> addRate(const Currency from, const Currency to, int32_t rate) const {
      std::array<Rate, N+1> rates{ Rate{Hash{from, to}, rate} };
      std::array<TransrationRate, N+1> transration_rates;

      for (size_t i = 0; i < N; ++i)
        transration_rates[i] = transration_rates_[i];
      transration_rates[N] = TransrationRate(from, to, rate);
 
      return Bank<N+1>{{rates}};
    }
    constexpr int32_t rate(const Currency from, const Currency to) const {
      if (from == to) return 1;
      return findRate(from, to);
    }

 private:
    constexpr int32_t findRate(const Currency from, const Currency to) const {
       for (auto i = 0; i < N; ++i) {
        auto hash = rates_[i].first;
        if (hash.first == from && hash.second == to)
          return rates_[i].second;
      }
      return 0;
    }

 private:
    std::array<Rate, N> rates_;
    std::array<TransrationRate, N> transration_rates_;
};

template <class T, class U>
class Sum : public Expression<Sum<T, U>> {
 public:
    constexpr Sum(const T& augend, const U& addend)
        : augend_{augend}, addend_{addend} {}

    // Implements Expression interface.
    template <int32_t N>
    constexpr Money reduce(const Bank<N>& bank, const Currency to) const {
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
template <int32_t N>
constexpr Money Expression<Derived>::reduce(const Bank<N>& bank, const Currency to)const {
  return static_cast<const Derived&>(*this).reduce(bank, to);
}

template <int32_t N>
constexpr Money Money::reduce(const Bank<N>& bank, const Currency to) const {
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
