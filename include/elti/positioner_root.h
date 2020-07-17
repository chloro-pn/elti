#pragma once

#include "key.h"
#include "util.h"
#include "positioner.h"
#include <cassert>

namespace elti {
class PositionerRoot {
public:
  PositionerRoot(const char* ptr):ptr_(ptr) {
    Key key("");
    size_t offset = 0;
    key.keyParse(ptr_, offset);
    type_ = parseValueType(ptr_, offset);
  }

  Positioner operator[](const char* attr) {
    return Positioner(ptr_, type_)[attr];
  }

  Positioner operator[](num n) {
    return Positioner(ptr_, type_)[n];
  }

  template<typename T>
  T get() {
    return Positioner(ptr_, ValueType::Data).get<T>();
  }

private:
  const char* ptr_;
  ValueType type_;
};
}
