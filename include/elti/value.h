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
#include "ref.h"

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
  //for ref.
  const void* ptr_;
  size_t length_;

  enum class type { DATA, REF, INVALID };
  type type_;

public:
  Data();

  //万能引用的匹配能力过强，需要通过enable_if为ref提供特殊实现。
  template<typename T,
    typename std::enable_if<!std::is_same_v<ref, std::decay_t<T>>, int>::type = 0>
  explicit Data(const T& obj) : Value(ValueType::Data), type_(type::DATA) {
    seri(obj, data_);
  }

  explicit Data(const ref& obj);

  //万能引用的匹配能力过强，需要通过enable_if为ref提供特殊实现。
  template<typename T, 
    typename std::enable_if<!std::is_same_v<ref, std::decay_t<T>>, int>::type = 0>
  Data& operator=(T&& other) {
    type_ = type::DATA;
    seri(std::forward<T>(other), data_);
    return *this;
  }

  Data& operator=(const ref& obj);

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
