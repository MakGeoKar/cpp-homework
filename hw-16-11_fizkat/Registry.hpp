#pragma once

#include <ostream>
#include <stdexcept>
#include <vector>

template <typename T, int Cap>
class Registry {
  static_assert(Cap > 0, "Registry capacity must be positive");

 public:
  void add(const T& entry) {
    if (size() == Cap) {
      entries_.erase(entries_.begin());
    }
    entries_.push_back(entry);
  }

  int size() const { return static_cast<int>(entries_.size()); }

  const T& operator[](int index) const {
    if (index < 0 || index >= size()) {
      throw std::out_of_range("Registry index out of range");
    }
    return entries_[index];
  }

 private:
  std::vector<T> entries_;
};

template <typename T, int Cap>
std::ostream& operator<<(std::ostream& os, const Registry<T, Cap>& registry) {
  for (int i = 0; i < registry.size(); ++i) {
    os << registry[i] << '\n';
  }
  return os;
}
