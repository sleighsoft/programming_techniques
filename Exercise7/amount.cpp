#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <cmath>

class Amount {
public:
  enum Currency { EUR, USD };
  double netto() { return netto_; };
  const double mwst() { return mwst_; };
  double brutto() { return brutto_; };
  double mwst_amount() { return mwst_amount_; };
  std::string text() { return text_; };
  Currency currency() { return currency_; };

  Amount(double netto, double mwst, std::string text = "A nice product",
         Currency currency = EUR)
      : netto_(netto), mwst_(mwst), text_(text), currency_(currency) {
    brutto_ = calculateBrutto();
    mwst_amount_ = calculateMwstAmount();
  }

  void currency(Currency currency) {
    assert(currency >= EUR && currency <= USD);
    calculateCurrencyConversion(currency);
    currency_ = currency;
  }

private:
  double netto_;
  double brutto_;
  double mwst_amount_;
  double mwst_;
  std::string text_;
  Currency currency_;

  void calculateCurrencyConversion(Currency newCurrency) {
    if (currency_ == newCurrency) {
      return;
    }
    double rate = getConversionRate(newCurrency);
    netto_ = netto_ * rate;
    brutto_ = calculateBrutto();
    mwst_amount_ = calculateMwstAmount();
  }

  double calculateMwstAmount() { return brutto_ - netto_; }

  double calculateBrutto() { return netto_ * (mwst_ / 100 + 1); }

  double getConversionRate(Currency newCurrency) {
    if (currency_ == EUR && newCurrency == USD) {
      return 1.1;
    } else if (currency_ == USD && newCurrency == EUR) {
      return 0.909091;
    } else {
      return 1.0;
    }
  }
};

bool almost_equals(double a, double b, double epsilon = 0.001) {
  return std::abs(a - b) < epsilon;
}

void test() {
  // implement tests
  double mwst = 19;
  std::string carText = "A nice car";
  Amount car(115, mwst, carText);
  assert(car.netto() == 115);
  assert(car.mwst() == 19);
  assert(carText.compare(car.text()) == 0);
  assert(car.currency() == Amount::Currency::EUR);
  assert(almost_equals(car.mwst_amount(), 21.85));
  car.currency(Amount::Currency::USD);
  assert(almost_equals(car.brutto(), 150.535));
  assert(car.currency() == Amount::Currency::USD);
  car.currency(Amount::Currency::USD);
  assert(almost_equals(car.brutto(), 150.535));
  assert(almost_equals(car.netto(), 126.5));
  car.currency(Amount::Currency::EUR);
  assert(almost_equals(car.netto(), 115));
  assert(car.currency() == Amount::Currency::EUR);

  std::string houseText = "A huge house";
  Amount house(2000, mwst, houseText, Amount::Currency::USD);
  assert(house.currency() == Amount::Currency::USD);
  assert(house.netto() == 2000);
}

int main() {
  test();
  return 0;
}