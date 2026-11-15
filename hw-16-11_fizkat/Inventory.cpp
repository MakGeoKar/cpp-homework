#include "Inventory.hpp"

#include <iomanip>
#include <sstream>

#include "Errors.hpp"

std::string EnergyConsumer::energyText() const {
  std::ostringstream out;
  out << std::fixed << std::setprecision(1) << dailyEnergy() << " kWh/day";
  return out.str();
}

double EnergyConsumer::checkedEnergy(double kwh) {
  if (kwh < 0) {
    throw InvalidValueError("energy must be >= 0");
  }
  return kwh;
}

std::string InventoryItem::checkedText(std::string value,
                                       const std::string& field) {
  if (value.empty()) {
    throw InvalidValueError(field + " must not be empty");
  }
  return value;
}

std::ostream& operator<<(std::ostream& os, const InventoryItem& item) {
  return os << item.inventoryId() << ' ' << item.name();
}

bool operator<(const InventoryItem& a, const InventoryItem& b) {
  return a.inventoryId() < b.inventoryId();
}
