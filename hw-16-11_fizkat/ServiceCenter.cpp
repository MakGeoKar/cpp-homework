#include "ServiceCenter.hpp"

#include <algorithm>
#include <cctype>

#include "Errors.hpp"

template <>
std::ostream& operator<<(std::ostream& os, const InspectionLog& log) {
  if (log.size() == 0) {
    return os << "no inspections yet\n";
  }
  for (int i = 0; i < log.size(); ++i) {
    os << log[i].id << ' ' << (log[i].accepted ? "accepted" : "rejected")
       << '\n';
  }
  return os;
}

std::unique_ptr<Transport> ServiceCenter::inspect(
    std::unique_ptr<Transport> unit) {
  std::string lowered = unit->name();
  std::transform(lowered.begin(), lowered.end(), lowered.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  bool accepted = lowered.find("broken") == std::string::npos;

  log_.add({unit->inventoryId(), accepted});
  if (!accepted) {
    throw InspectionError();
  }
  return unit;
}
