#include "Transport.hpp"

#include <utility>

#include "Errors.hpp"

Transport::Transport(std::string id, std::string name, int noviceScore)
    : id_(checkedText(std::move(id), "id")),
      name_(checkedText(std::move(name), "name")),
      noviceScore_(noviceScore) {
  if (noviceScore < 1 || noviceScore > 10) {
    throw InvalidValueError("novice score must be in 1..10");
  }
}

bool Transport::isNoviceFriendly() const {
  return noviceScore_ >= kNoviceMinScore;
}

std::string Bike::describe() const {
  return "bike, novice score " + std::to_string(noviceScore());
}

ElectricTransport::ElectricTransport(std::string id, std::string name,
                                     double dailyEnergy, int noviceScore)
    : Transport(std::move(id), std::move(name), noviceScore),
      dailyEnergy_(checkedEnergy(dailyEnergy)) {}

std::string ElectricTransport::details() const {
  return energyText() + ", novice score " + std::to_string(noviceScore());
}

std::string ElectricScooter::describe() const {
  return "electric scooter, " + details();
}

std::string ElectricBike::describe() const {
  return "electric bike, " + details();
}

std::string Monowheel::describe() const {
  return "monowheel, " + details() + ", never for novices";
}
