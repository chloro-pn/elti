#pragma once
#include "element.h"
#include "value.h"
#include "value_wrapper.h"

namespace elti {
class Root {
private:
  Element root_;

  Value* getValue() {
    return root_.getValue();
  }

public:
  explicit Root(Value* value) : root_("", value) {

  }

  Root() = default;

  Value* reset() {
    Value* tmp = root_.getValue();
    root_.v_ = nullptr;
    return tmp;
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

  ValueWrapper operator[](num n) {
    return ValueWrapper(getValueAsArray(getValue())->operator[](n.n_));
  }

  uint64_t size() {
    return getValueAsArray(getValue())->size();
  }

  template<typename T>
  T get() {
    return getValueAsData(getValue())->get<T>();
  }
};
}
