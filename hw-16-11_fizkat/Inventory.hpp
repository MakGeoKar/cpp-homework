#pragma once

#include <ostream>
#include <string>

class EnergyConsumer {
 public:
  virtual ~EnergyConsumer() = default;

  virtual double dailyEnergy() const = 0;

  std::string energyText() const;

 protected:
  static double checkedEnergy(double kwh);
};

class InventoryItem {
 public:
  virtual ~InventoryItem() = default;

  virtual const std::string& inventoryId() const = 0;
  virtual const std::string& name() const = 0;
  virtual std::string describe() const = 0;

  virtual const EnergyConsumer* asEnergyConsumer() const { return nullptr; }

  virtual bool isNoviceFriendly() const { return false; }

 protected:
  static std::string checkedText(std::string value, const std::string& field);
};

std::ostream& operator<<(std::ostream& os, const InventoryItem& item);

bool operator<(const InventoryItem& a, const InventoryItem& b);
