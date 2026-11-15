#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Errors.hpp"
#include "Fleet.hpp"
#include "ServiceCenter.hpp"
#include "Things.hpp"
#include "Transport.hpp"

namespace {

using Args = std::vector<std::string>;

void expectArgs(const Args& args, std::size_t count, const std::string& usage) {
  if (args.size() != count) {
    throw CommandError("usage: " + usage);
  }
}

int parseInt(const std::string& text) {
  std::istringstream in(text);
  int value = 0;
  if (!(in >> value) || !(in >> std::ws).eof()) {
    throw CommandError("not an integer: " + text);
  }
  return value;
}

double parseDouble(const std::string& text) {
  std::istringstream in(text);
  double value = 0;
  if (!(in >> value) || !(in >> std::ws).eof()) {
    throw CommandError("not a number: " + text);
  }
  return value;
}

void addCommand(const Args& args, Fleet& fleet, ServiceCenter& service) {
  if (args.size() < 2) {
    throw CommandError("usage: add <type> <id> <name> ...");
  }
  const std::string& type = args[1];
  if (type == "scooter") {
    expectArgs(args, 6, "add scooter <id> <name> <energy> <score>");
    fleet.addTransport(
        std::make_unique<ElectricScooter>(args[2], args[3], parseDouble(args[4]),
                                          parseInt(args[5])),
        service);
  } else if (type == "ebike") {
    expectArgs(args, 6, "add ebike <id> <name> <energy> <score>");
    fleet.addTransport(
        std::make_unique<ElectricBike>(args[2], args[3], parseDouble(args[4]),
                                       parseInt(args[5])),
        service);
  } else if (type == "monowheel") {
    expectArgs(args, 6, "add monowheel <id> <name> <energy> <score>");
    fleet.addTransport(
        std::make_unique<Monowheel>(args[2], args[3], parseDouble(args[4]),
                                    parseInt(args[5])),
        service);
  } else if (type == "bike") {
    expectArgs(args, 5, "add bike <id> <name> <score>");
    fleet.addTransport(
        std::make_unique<Bike>(args[2], args[3], parseInt(args[4])), service);
  } else if (type == "helmet") {
    expectArgs(args, 4, "add helmet <id> <name>");
    fleet.addThing(std::make_unique<Helmet>(args[2], args[3]));
  } else if (type == "dock") {
    expectArgs(args, 5, "add dock <id> <name> <energy>");
    fleet.addThing(
        std::make_unique<Dock>(args[2], args[3], parseDouble(args[4])));
  } else {
    throw CommandError("unknown type: " + type);
  }
}

void execute(const Args& args, Fleet& fleet, ServiceCenter& service) {
  const std::string& command = args[0];
  if (command == "add") {
    addCommand(args, fleet, service);
  } else if (command == "list") {
    expectArgs(args, 1, "list");
    fleet.printAll(std::cout);
  } else if (command == "novice") {
    expectArgs(args, 1, "novice");
    fleet.printNovice(std::cout);
  } else if (command == "energy") {
    expectArgs(args, 1, "energy");
    std::cout << std::fixed << std::setprecision(1) << fleet.totalEnergy()
              << '\n';
  } else if (command == "report") {
    expectArgs(args, 1, "report");
    std::cout << fleet.size() << " items, " << std::fixed
              << std::setprecision(1) << fleet.totalEnergy() << " kWh/day\n";
  } else if (command == "focus") {
    expectArgs(args, 2, "focus <id>");
    fleet.setFocus(args[1]);
  } else if (command == "show_focus") {
    expectArgs(args, 1, "show_focus");
    fleet.printFocus(std::cout);
  } else if (command == "remove") {
    expectArgs(args, 2, "remove <id>");
    fleet.remove(args[1]);
  } else if (command == "log") {
    expectArgs(args, 1, "log");
    std::cout << service.log();
  } else {
    throw CommandError("unknown command: " + command);
  }
}

}  // namespace

int main() {
  Fleet fleet;
  ServiceCenter service;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream in(line);
    Args args;
    for (std::string word; in >> word;) {
      args.push_back(word);
    }
    if (args.empty()) {
      continue;
    }
    if (args[0] == "quit") {
      break;
    }
    try {
      execute(args, fleet, service);
    } catch (const FleetError& e) {
      std::cout << "error: " << e.what() << '\n';
    }
  }
  return 0;
}
