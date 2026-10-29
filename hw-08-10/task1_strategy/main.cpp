#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using DiscountStrategy = std::function<double(double)>;

double NoDiscount(double sum) { return sum; }

DiscountStrategy Percent(double percent) {
  return [percent](double sum) { return sum * (100 - percent) / 100; };
}

DiscountStrategy Fixed(double amount) {
  return [amount](double sum) { return std::max(0.0, sum - amount); };
}

struct FromSum {
  double threshold;
  double percent;

  double operator()(double sum) const {
    if (sum < threshold) {
      return sum;
    }
    return sum * (100 - percent) / 100;
  }
};

class Cart {
 public:
  explicit Cart(DiscountStrategy discount) : discount_(std::move(discount)) {}

  void Add(const std::string& name, double price) {
    items_.push_back({name, price});
  }

  void SetDiscount(DiscountStrategy discount) {
    discount_ = std::move(discount);
  }

  double Sum() const {
    double sum = 0;
    for (const Item& item : items_) {
      sum += item.price;
    }
    return sum;
  }

  double Total() const { return discount_(Sum()); }

 private:
  struct Item {
    std::string name;
    double price;
  };

  std::vector<Item> items_;
  DiscountStrategy discount_;
};

int main() {
  Cart cart(NoDiscount);
  cart.Add("book", 700);
  cart.Add("pen", 50);
  cart.Add("notebook", 250);

  std::vector<std::pair<std::string, DiscountStrategy>> strategies = {
      {"no discount", NoDiscount},
      {"10%", Percent(10)},
      {"-300", Fixed(300)},
      {"-2000", Fixed(2000)},
      {"15% from 500", FromSum{500, 15}},
      {"15% from 5000", FromSum{5000, 15}},
      {"student", [](double sum) { return sum * 0.8; }},
  };

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "sum: " << cart.Sum() << "\n";
  for (const auto& [name, strategy] : strategies) {
    cart.SetDiscount(strategy);
    std::cout << std::left << std::setw(15) << name << " -> " << cart.Total()
              << "\n";
  }
  return 0;
}
