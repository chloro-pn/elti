#pragma once

#include "elti/key.h"
#include "elti/util.h"
#include "elti/value_wrapper.h"
#include <vector>

namespace elti {
class Positioner {
public:
  Positioner(const char* ptr, ValueType type, bool find = true);
  Positioner operator[](const char* attr);
  Positioner operator[](num index);

  bool IsFind() const {
    return find_;
  }

  uint64_t size() const {
    size_t offset = 0;
    const char* current_value = ptr_;
    uint64_t count = parseLength(current_value, offset);
    return count;
  }

  template<typename T>
  T get() {
    assert(type_ == ValueType::Data);
    return parse<T>(ptr_, static_cast<size_t>(total_size_));
  }

  uint64_t getTotalSize() const {
    return total_size_;
  }

  ValueType getType() const {
    return type_;
  }

private:
  const char* ptr_;
  ValueType type_;
  uint64_t total_size_;
  bool find_;

  const char* getNextValueAddr() const {
    return ptr_ + total_size_;
  }
};
}
