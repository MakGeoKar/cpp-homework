#include <cstring>
#include <iomanip>
#include <iostream>

typedef void (*Operation)(int&);

// k читаем внутри deposit / withdraw, чтобы у всех функций был один тип
bool ReadAmount(int& k) {
  std::cin >> k;
  if (!std::cin) {
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    k = 0;
  }
  return k > 0;
}

void Deposit(int& balance) {
  int k;
  if (!ReadAmount(k)) {
    std::cout << "error\n";
    return;
  }
  balance += k;
}

void Withdraw(int& balance) {
  int k;
  if (!ReadAmount(k) || k > balance) {
    std::cout << "error\n";
    return;
  }
  balance -= k;
}

void Balance(int& balance) { std::cout << balance << "\n"; }

void Reset(int& balance) { balance = 0; }

// своя команда: удваивает сумму
void Double(int& balance) { balance *= 2; }

const int kCommandsCount = 5;
const char* kNames[kCommandsCount] = {"deposit", "withdraw", "balance", "reset",
                                      "double"};
Operation kOperations[kCommandsCount] = {Deposit, Withdraw, Balance, Reset,
                                         Double};

int main() {
  int balance = 0;
  char cmd[32];

  while (std::cin >> std::setw(32) >> cmd) {
    if (strcmp(cmd, "stop") == 0) {
      break;
    }

    bool found = false;
    for (int i = 0; i < kCommandsCount; ++i) {
      if (strcmp(cmd, kNames[i]) == 0) {
        kOperations[i](balance);
        found = true;
        break;
      }
    }
    if (!found) {
      std::cout << "error\n";
      std::cin.ignore(1000, '\n');
    }
  }
  return 0;
}
