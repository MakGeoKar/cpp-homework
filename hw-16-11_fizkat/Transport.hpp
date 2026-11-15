#pragma once

#include <string>

#include "Inventory.hpp"

constexpr int kNoviceMinScore = 6;

class Transport : public InventoryItem {
 public:
  Transport(std::string id, std::string name, int noviceScore);

  const std::string& inventoryId() const override { return id_; }
  const std::string& name() const override { return name_; }
  bool isNoviceFriendly() const override;

  int noviceScore() const { return noviceScore_; }

 private:
  std::string id_;
  std::string name_;
  int noviceScore_;
};

class Bike : public Transport {
 public:
  using Transport::Transport;
  std::string describe() const override;
};

class ElectricTransport : public Transport, public EnergyConsumer {
 public:
  ElectricTransport(std::string id, std::string name, double dailyEnergy,
                    int noviceScore);

  double dailyEnergy() const override { return dailyEnergy_; }
  const EnergyConsumer* asEnergyConsumer() const override { return this; }

 protected:
  std::string details() const;

 private:
  double dailyEnergy_;
};

class ElectricScooter : public ElectricTransport {
 public:
  using ElectricTransport::ElectricTransport;
  std::string describe() const override;
};

class ElectricBike : public ElectricTransport {
 public:
  using ElectricTransport::ElectricTransport;
  std::string describe() const override;
};

class Monowheel : public ElectricTransport {
 public:
  using ElectricTransport::ElectricTransport;
  std::string describe() const override;
  bool isNoviceFriendly() const override { return false; }
};
