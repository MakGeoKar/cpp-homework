#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "Inventory.hpp"
#include "ServiceCenter.hpp"
#include "Things.hpp"
#include "Transport.hpp"

class Fleet {
 public:
  void addThing(std::unique_ptr<Thing> thing);
  void addTransport(std::unique_ptr<Transport> unit, ServiceCenter& service);
  void remove(const std::string& id);

  void setFocus(const std::string& id);
  void printFocus(std::ostream& os) const;

  void printAll(std::ostream& os);
  void printNovice(std::ostream& os);

  double totalEnergy() const;
  int size() const { return static_cast<int>(items_.size()); }

 private:
  void ensureNewId(const std::string& id) const;
  void sortById();

  std::vector<std::shared_ptr<InventoryItem>> items_;
  std::weak_ptr<InventoryItem> focus_;
  bool focusSet_ = false;
};
