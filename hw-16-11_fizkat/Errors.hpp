#pragma once

#include <stdexcept>
#include <string>

class FleetError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class InspectionError : public FleetError {
 public:
  InspectionError() : FleetError("inspection rejected") {}
};

class CommandError : public FleetError {
 public:
  using FleetError::FleetError;
};

class InvalidValueError : public FleetError {
 public:
  using FleetError::FleetError;
};

class NotFoundError : public FleetError {
 public:
  explicit NotFoundError(const std::string& id)
      : FleetError("no item with id " + id) {}
};
