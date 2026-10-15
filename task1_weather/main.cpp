#include <iomanip>
#include <iostream>

struct MinMax {
  int min;
  int max;
};

double Average(const int* temps, int n) {
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    sum += temps[i];
  }
  return static_cast<double>(sum) / n;
}

MinMax FindMinMax(const int* temps, int n) {
  MinMax res = {temps[0], temps[0]};
  for (int i = 1; i < n; ++i) {
    if (temps[i] < res.min) {
      res.min = temps[i];
    }
    if (temps[i] > res.max) {
      res.max = temps[i];
    }
  }
  return res;
}

int CountFrostDays(const int* temps, int n) {
  int count = 0;
  for (int i = 0; i < n; ++i) {
    if (temps[i] < 0) {
      ++count;
    }
  }
  return count;
}

int main() {
  int n;
  std::cin >> n;
  if (n <= 0) {
    return 0;
  }

  int* temps = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> temps[i];
  }

  MinMax mm = FindMinMax(temps, n);
  std::cout << std::fixed << std::setprecision(1) << Average(temps, n) << "\n";
  std::cout << mm.min << " " << mm.max << "\n";
  std::cout << CountFrostDays(temps, n) << "\n";

  delete[] temps;
  return 0;
}
