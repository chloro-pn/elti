#include "elti/positioner.h"
#include "elti/key.h"
#include "elti/elti_enum.h"
#include <cassert>
#include <cstring>

namespace elti {
Positioner::Positioner(const char* ptr, ValueType type, bool find) :
  ptr_(ptr), type_(type), total_size_(0), find_(find) {
  size_t offset;
  if(type != ValueType::Invalid) {
    total_size_ = parseLength(ptr_, offset);
  }
}

Positioner Positioner::operator[](const char *attr) {
  assert(type_ == ValueType::Map);
  size_t offset = 0;
  const char* current_value = ptr_;
  uint64_t count = parseLength(current_value, offset);
  for(uint64_t i = 0; i < count; ++i) {
    Key key("");
    key.keyParse(current_value, offset);
    ValueType type = parseValueType(current_value, offset);
    if(key == Key(attr, strlen(attr))) {
      return Positioner(current_value, type);
    }
    else {
      uint64_t total_size = parseLength(current_value, offset);
      current_value += total_size;
    }
  }
  return Positioner(nullptr, ValueType::Invalid, false);
}

Positioner Positioner::operator[](num index) {
  assert(type_ == ValueType::Array);
  size_t offset = 0;
  const char* current_value = ptr_;
  uint64_t count = parseLength(current_value, offset);
  if(index.n_ >= count) {
    return Positioner(nullptr, ValueType::Invalid, false);
  }
  for(uint64_t i = 0; i < count; ++i) {
    ValueType type = parseValueType(current_value, offset);
    if(i < index.n_) {
      uint64_t total_size = parseLength(current_value, offset);
      current_value += total_size;
    }
    else {
      assert(i == index.n_);
      return Positioner(current_value, type);
    }
  }
  return Positioner(nullptr, ValueType::Invalid, false);
}
}
