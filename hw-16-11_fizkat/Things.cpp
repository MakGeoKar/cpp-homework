#include "Things.hpp"

#include <utility>

Thing::Thing(std::string id, std::string name)
    : id_(checkedText(std::move(id), "id")),
      name_(checkedText(std::move(name), "name")) {}

std::string Helmet::describe() const { return "helmet"; }

Dock::Dock(std::string id, std::string name, double dailyEnergy)
    : Thing(std::move(id), std::move(name)),
      dailyEnergy_(checkedEnergy(dailyEnergy)) {}

std::string Dock::describe() const { return "dock station, " + energyText(); }
