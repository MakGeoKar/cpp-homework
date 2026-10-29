#include <functional>
#include <iostream>
#include <map>
#include <memory>
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

Func Cached(Func f) {
  auto cache = std::make_shared<std::map<int, long long>>();
  return [f, cache](int x) {
    auto it = cache->find(x);
    if (it != cache->end()) {
      return it->second;
    }
    long long result = f(x);
    (*cache)[x] = result;
    return result;
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

  std::cout << "logged(cached):\n";
  Func a = Logged("square", Cached(Square));
  a(7);
  a(7);
  std::cout << "cached(logged):\n";
  Func b = Cached(Logged("square", Square));
  b(7);
  b(7);

  calls = 0;
  Func fast_fib;
  fast_fib = Cached(Counted(FibStep(fast_fib), calls));
  long long fast = fast_fib(30);
  std::cout << "cached fib(30) = " << fast << ", calls: " << calls << "\n";
  fast = fast_fib(90);
  std::cout << "cached fib(90) = " << fast << ", calls: " << calls << "\n";
  return 0;
}
