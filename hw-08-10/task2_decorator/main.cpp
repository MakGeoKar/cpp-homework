#include <functional>
#include <iostream>
#include <string>

using Func = std::function<long long(int)>;

Func Logged(const std::string& name, Func f) {
  return [name, f](int x) {
    long long result = f(x);
    std::cout << "  " << name << "(" << x << ") = " << result << "\n";
    return result;
  };
}

Func Counted(Func f, int& calls) {
  return [f, &calls](int x) {
    ++calls;
    return f(x);
  };
}

long long Square(int x) { return 1LL * x * x; }

Func FibStep(const Func& self) {
  return [&self](int n) -> long long {
    return n < 2 ? n : self(n - 1) + self(n - 2);
  };
}

int main() {
  int calls = 0;
  Func square = Logged("square", Counted(Square, calls));
  square(3);
  square(12);
  square(3);
  std::cout << "square calls: " << calls << "\n";

  calls = 0;
  Func fib;
  fib = Counted(FibStep(fib), calls);
  long long slow = fib(30);
  std::cout << "fib(30) = " << slow << ", calls: " << calls << "\n";
  return 0;
}
