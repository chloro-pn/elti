#pragma once
#include "element.h"
#include "value.h"
#include "value_wrapper.h"

namespace elti {
class Root {
private:
  Element root_;

public:
  explicit Root(Value* value) : root_("/", value) {

  }

  Root() {}

  Value* getValue() {
    return root_.getValue();
  }

  void seri(std::string& result) {
    root_.seri(result);
  }

  size_t parse(const char* ptr) {
    size_t offset = 0;
    root_.parse(ptr, offset);
    return offset;
  }

  ValueWrapper operator[](const char* attr) {
    return ValueWrapper(getValueAsMap(getValue())->operator[](attr));
  }

  ValueWrapper operator[](size_t n) {
    return ValueWrapper(getValueAsArray(getValue())->operator[](n));
  }

  uint64_t size() {
    return getValueAsArray(getValue())->getSize();
  }

  template<typename T>
  T get() {
    return getValueAsData(getValue())->get<T>();
  }
};
}
