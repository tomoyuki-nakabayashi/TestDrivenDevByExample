// Copyright <2018> <Tomoyuki Nakabayashi>
// This software is released under the MIT License, see LICENSE.

#ifndef CPP_TEMPLATE_MONEY_H_
#define CPP_TEMPLATE_MONEY_H_

#include <cstdint>
#include <type_traits>
#include <utility>
#include <array>

namespace money {

class Sum;
class Money;
template <int32_t N>
class Bank;

enum class Currency {
  kUSD, kCHF, kNoCurrency
};

template <class Derived>
class Expression {
 public:
    constexpr Expression() {}
    template <int32_t N>
    constexpr Money reduce(const Bank<N>& bank, const Currency to) const;
};

class Money : public Expression<Money> {
 public:
    constexpr Money(int32_t amount, Currency currency = Currency::kNoCurrency)
        : amount_{amount}, currency_{currency} {}
    constexpr Currency currency() const {
      return currency_;
    }
    constexpr int32_t amount() const{
      return amount_;
    }

    template <int32_t N>
    constexpr Money reduce(const Bank<N>& bank, const Currency to) const;

    constexpr friend bool operator==(const Money& rhs, const Money& lhs) {
      return (rhs.amount_ == lhs.amount_) && (rhs.currency_ == lhs.currency_);
    }

    constexpr friend Sum operator+(const Money& rhs, const Money& lhs);

    constexpr friend Money operator*(const Money& rhs, int32_t multiplier) {
      return Money{rhs.amount_*multiplier, rhs.currency_};
    }

 private:
    int32_t amount_;
    Currency currency_;
};

template <int32_t N>
class Bank {
  using Hash = std::pair<const Currency, const Currency>;
  using Rate = std::pair<Hash, int>;

 public:
    constexpr Bank(const std::array<Rate, N>& rates) : rates_{rates} {}
 
    template <class T>
    constexpr Money reduce(const Expression<T>& source, const Currency to) const {
      return source.reduce(*this, to);
    }
    constexpr Bank<N+1> addRate(const Currency from, const Currency to, int32_t rate) const {
      std::array<Rate, 1> rates = { Rate{Hash{from, to}, rate} };
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
};

template <class Derived>
template <int32_t N>
constexpr Money Expression<Derived>::reduce(const Bank<N>& bank, const Currency to)const {
  return static_cast<const Derived&>(*this).reduce(bank, to);
}

class Sum : public Expression<Sum> {
 public:
    constexpr Sum(const Money& augend, const Money& addend)
        : augend_{augend}, addend_{addend} {}
    template <int32_t N>
    constexpr Money reduce(const Bank<N>& bank, const Currency to) const {
      auto amount = augend_.reduce(bank, to).amount() + addend_.reduce(bank, to).amount();
      return Money{amount, to};
    }

 public:
    Money augend_;
    Money addend_;
};

template <int32_t N>
constexpr Money Money::reduce(const Bank<N>& bank, const Currency to) const {
  int rate = bank.rate(currency_, to);
  return Money(amount_ / rate, to);
}

constexpr Money dollar(int32_t amount) {
  return Money{amount, Currency::kUSD};
}

constexpr Money franc(int32_t amount) {
  return Money{amount, Currency::kCHF};
}

constexpr Sum operator+(const Money& rhs, const Money& lhs) {
  return Sum{rhs, lhs};
}

}  // namespace money

#endif  // CPP_TEMPLATE_MONEY_H_
