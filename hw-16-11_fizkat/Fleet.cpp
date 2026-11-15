#include "Fleet.hpp"

#include <algorithm>
#include <utility>

#include "Errors.hpp"

namespace {

auto hasId(const std::string& id) {
  return [&id](const std::shared_ptr<InventoryItem>& item) {
    return item->inventoryId() == id;
  };
}

}  // namespace

void Fleet::addThing(std::unique_ptr<Thing> thing) {
  ensureNewId(thing->inventoryId());
  items_.push_back(std::shared_ptr<InventoryItem>(std::move(thing)));
}

void Fleet::addTransport(std::unique_ptr<Transport> unit,
                         ServiceCenter& service) {
  ensureNewId(unit->inventoryId());
  std::unique_ptr<Transport> accepted = service.inspect(std::move(unit));
  items_.push_back(std::shared_ptr<InventoryItem>(std::move(accepted)));
}

void Fleet::remove(const std::string& id) {
  auto it = std::find_if(items_.begin(), items_.end(), hasId(id));
  if (it == items_.end()) {
    throw NotFoundError(id);
  }
  items_.erase(it);
}

void Fleet::setFocus(const std::string& id) {
  auto it = std::find_if(items_.begin(), items_.end(), hasId(id));
  if (it == items_.end()) {
    throw NotFoundError(id);
  }
  focus_ = *it;
  focusSet_ = true;
}

void Fleet::printFocus(std::ostream& os) const {
  if (std::shared_ptr<InventoryItem> item = focus_.lock()) {
    os << *item << " (" << item->describe() << ")\n";
  } else if (focusSet_) {
    os << "removed\n";
  } else {
    os << "no focus\n";
  }
}

void Fleet::printAll(std::ostream& os) {
  sortById();
  for (const auto& item : items_) {
    os << *item << '\n';
  }
}

void Fleet::printNovice(std::ostream& os) {
  sortById();
  for (const auto& item : items_) {
    if (item->isNoviceFriendly()) {
      os << *item << '\n';
    }
  }
}

double Fleet::totalEnergy() const {
  double total = 0;
  for (const auto& item : items_) {
    if (const EnergyConsumer* consumer = item->asEnergyConsumer()) {
      total += consumer->dailyEnergy();
    }
  }
  return total;
}

void Fleet::ensureNewId(const std::string& id) const {
  if (std::any_of(items_.begin(), items_.end(), hasId(id))) {
    throw InvalidValueError("duplicate id " + id);
  }
}

void Fleet::sortById() {
  std::sort(items_.begin(), items_.end(),
            [](const std::shared_ptr<InventoryItem>& a,
               const std::shared_ptr<InventoryItem>& b) { return *a < *b; });
}
