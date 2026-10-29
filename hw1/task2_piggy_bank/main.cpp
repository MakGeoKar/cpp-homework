#include <cstring>
#include <iostream>

typedef void (*Operation)(int&);

void Deposit(int& balance) {
  int k;
  std::cin >> k;
  if (k <= 0) {
    std::cout << "error\n";
    return;
  }
  balance += k;
}

void Withdraw(int& balance) {
  int k;
  std::cin >> k;
  if (k <= 0 || k > balance) {
    std::cout << "error\n";
    return;
  }
  balance -= k;
}

void Balance(int& balance) { std::cout << balance << "\n"; }

void Reset(int& balance) { balance = 0; }

void Double(int& balance) { balance *= 2; }

const int kCommandsCount = 5;
const char* kNames[kCommandsCount] = {"deposit", "withdraw", "balance", "reset",
                                      "double"};
Operation kOperations[kCommandsCount] = {Deposit, Withdraw, Balance, Reset,
                                         Double};

int main() {
  int balance = 0;
  char cmd[32];

  while (std::cin >> cmd) {
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
    }
  }
  return 0;
}
