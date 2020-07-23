#pragma once
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "varint.h"
#include "element.h"
#include "value_type.h"
#include "seri_parse.h"

namespace elti {
class Value {
private:
  ValueType type_;

public:
  explicit Value(ValueType type);

  ValueType getType() const {
    return type_;
  }

  void valueParse(const char*& begin, size_t& offset);

  void valueSeri(std::string& result);

  Value() = default;
};

Value* valueFactory(ValueType type);

class Map : public Value {
private:
  std::vector<Element> kvs_;

public:
  Map();
  void valueParse(const char*& begin, size_t& offset);

  void valueSeri(std::string& result);

  void set(const char* attr, Value* v);

  void erase(const char* attr);

  bool contains(const char* attr);

  Value* operator[](const char* attr);

  ~Map() = default;
};

class Array : public Value {
private:
  std::vector<Value*> vs_;

public:
  Array();

  void valueParse(const char*& begin, size_t& offset);

  void valueSeri(std::string& result);

  uint64_t size();

  void push_back(Value* v);

  void erase(size_t n);

  Value* operator[](size_t index);

  ~Array();
};

class Data : public Value {
private:
  std::vector<uint8_t> data_;

public:
  Data();

  template<typename T>
  explicit Data(const T& obj) : Value(ValueType::Data) {
    seri(obj, data_);
  }

  template<typename T>
  Data& operator=(T&& other) {
    seri(std::forward<T>(other), data_);
    return *this;
  }

  void valueParse(const char*& begin, size_t& offset);

  void valueSeri(std::string& result);

  size_t useBytes() const;

  std::vector<uint8_t>& bytesRef();

  template<typename T>
  T get() {
    return parse<T>(data_);
  }

  ~Data() = default;
};
}
