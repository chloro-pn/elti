#pragma once
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "varint.h"
#include "element.h"
#include "util.h"

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

  virtual ~Value() {

  }
};

Value* valueFactory(ValueType type);

class Map : public Value {
private:
  std::vector<Element> kvs_;

public:
  Map();
  void valueParse(const char*& begin, size_t& offset) {
    parseLength(begin, offset); //total size.
    uint64_t count = parseLength(begin, offset);
    for(uint64_t i = 0; i < count; ++i) {
      Element tmp;
      tmp.parse(begin, offset);
      CHECK_PTR(begin);
      kvs_.push_back(std::move(tmp));
    }
  }

  void valueSeri(std::string& result) {
    std::string tmp;
    uint32_t count = kvs_.size();
    seriLength(count, tmp);
    for(auto& each : kvs_) {
      each.seri(tmp);
    }
    seriLength(tmp.size(), result);
    result.append(tmp);
  }

  void setElements(std::vector<Element>&& eles) {
    kvs_.clear();
    for(auto& each : eles) {
      kvs_.push_back(std::move(each));
    }
    eles.clear();
  }

  void set(const char* attr, Value* v) {
    auto it = std::find_if(kvs_.begin(), kvs_.end(), [attr](const Element& e)->bool {
      return strcmp(attr, e.getKey()) == 0;
    });

    if(it != kvs_.end()) {
      kvs_.erase(it);
    }
    kvs_.push_back(Element(attr, v));
  }

  bool contains(const char* attr) {
    for(auto& each : kvs_) {
      if(strcmp(attr, each.getKey()) == 0) {
        return true;
      }
    }
    return false;
  }

  Value* operator[](const char* attr) {
    for(auto& each : kvs_) {
      if(strcmp(attr, each.getKey()) == 0) {
        return each.v_;
      }
    }
    return nullptr;
  }

  ~Map() = default;
};

class Array : public Value {
private:
  std::vector<Value*> vs_;

public:
  Array();

  void valueParse(const char*& begin, size_t& offset) {
    parseLength(begin, offset); //total size.
    uint64_t count = parseLength(begin, offset);
    for(uint64_t i = 0; i < count; ++i) {
      ValueType type = parseValueType(begin, offset);
      Value* v = valueFactory(type);
      v->valueParse(begin, offset);
      vs_.push_back(v);
    }
  }

  void valueSeri(std::string& result) {
    std::string tmp;
    uint32_t count = vs_.size();
    seriLength(count, tmp);
    for(auto& each : vs_) {
      seriValueType(each->getType(), tmp);
      each->valueSeri(tmp);
    }
    seriLength(tmp.size(), result);
    result.append(tmp);
  }

  void setArray(std::vector<Value*>&& vs) {
    vs_.clear();
    for(auto& each : vs) {
      vs_.push_back(each);
    }
    vs.clear();
  }

  uint64_t getSize() {
    return vs_.size();
  }

  void push_back(Value* v) {
    vs_.push_back(v);
  }

  Value* operator[](size_t index) {
    return vs_[index];
  }

  ~Array() {
    for(auto& each_value_ptr : vs_) {
      delete each_value_ptr;
    }
  }
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

  void valueParse(const char*& begin, size_t& offset) {
    uint64_t size = parseLength(begin, offset);
    data_.resize(size);
    memcpy(&(*data_.begin()), begin, size);
    begin += size;
    offset += size;
  }

  void valueSeri(std::string& result) {
    uint32_t length = data_.size();
    seriLength(length, result);
    size_t old_len = result.size();
    result.resize(result.size() + length);
    memcpy(&(*(result.begin() + old_len)), &data_.front(), length);
  }

  size_t useBytes() const {
    return data_.size();
  }

  std::vector<uint8_t>& bytesRef() {
    return data_;
  }

  template<typename T>
  T get() {
    return parse<T>(data_);
  }
};
}
