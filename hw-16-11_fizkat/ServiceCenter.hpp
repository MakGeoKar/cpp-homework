#pragma once

#include <memory>
#include <ostream>
#include <string>

#include "Registry.hpp"
#include "Transport.hpp"

struct InspectionRecord {
  std::string id;
  bool accepted;
};

using InspectionLog = Registry<InspectionRecord, 16>;

template <>
std::ostream& operator<<(std::ostream& os, const InspectionLog& log);

class ServiceCenter {
 public:
  std::unique_ptr<Transport> inspect(std::unique_ptr<Transport> unit);

  const InspectionLog& log() const { return log_; }

 private:
  InspectionLog log_;
};
