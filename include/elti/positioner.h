#pragma once

#include "elti/util.h"
#include "elti/elti_num.h"
#include "elti/elti_type_trait.h"
#include "elti/seri_parse.h"
#include <vector>
#include <string>
#include <cassert>

namespace elti {
class Positioner {
public:
  Positioner(const char* ptr, ValueType type, bool find = true);
  Positioner operator[](const char* attr);
  Positioner operator[](num index);

  uint64_t size() const {
    assert(type_ == ValueType::Array);
    size_t offset = 0;
    const char* current_value = ptr_;
    uint64_t count = parseLength(current_value, offset);
    return count;
  }

  template<typename T>
  T get() const {
    static_assert (detail::NoRefNoPointer<T>::value, "get() method should not return & or * type"
                                                     "(including <T* const> and <const T*> type).\n");
    assert(type_ == ValueType::Data);
    size_t offset = 0;
    const char* tmp = ptr_;
    parseDataType(tmp, offset);
    return parse<T>(tmp, static_cast<size_t>(total_size_ - 1));
  }

  bool IsFind() const {
    return find_;
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
