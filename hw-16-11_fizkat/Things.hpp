#pragma once

#include <string>

#include "Inventory.hpp"

class Thing : public InventoryItem {
 public:
  Thing(std::string id, std::string name);

  const std::string& inventoryId() const override { return id_; }
  const std::string& name() const override { return name_; }

 private:
  std::string id_;
  std::string name_;
};

class Helmet : public Thing {
 public:
  using Thing::Thing;
  std::string describe() const override;
};

class Dock : public Thing, public EnergyConsumer {
 public:
  Dock(std::string id, std::string name, double dailyEnergy);

  std::string describe() const override;
  double dailyEnergy() const override { return dailyEnergy_; }
  const EnergyConsumer* asEnergyConsumer() const override { return this; }

 private:
  double dailyEnergy_;
};
