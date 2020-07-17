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

  const char* getNextValueAddr() const {
    return ptr_ + total_size_;
  }

  bool IsFind() const {
    return find_;
  }

  template<typename T>
  T get() {
    assert(type_ == ValueType::Data);
    std::vector<uint8_t> tmp;
    tmp.resize(total_size_);
    memcpy(&tmp.front(), ptr_, total_size_);
    return parse<T>(tmp);
  }

private:
  const char* ptr_;
  ValueType type_;
  uint64_t total_size_;
  bool find_;
};
}
