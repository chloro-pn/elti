#pragma once
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
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
    uint64_t count = parseLength(begin, offset);
    for(uint64_t i = 0; i < count; ++i) {
      Element tmp;
      tmp.parse(begin, offset);
      CHECK_PTR(begin);
      kvs_.push_back(std::move(tmp));
    }
  }

  void valueSeri(std::string& result) {
    uint32_t count = kvs_.size();
    seriLength(count, result);
    for(auto& each : kvs_) {
      each.seri(result);
    }
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
    uint64_t count = parseLength(begin, offset);
    for(uint64_t i = 0; i < count; ++i) {
      ValueType type = parseValueType(begin, offset);
      Value* v = valueFactory(type);
      v->valueParse(begin, offset);
      vs_.push_back(v);
    }
  }

  void valueSeri(std::string& result) {
    uint32_t count = vs_.size();
    seriLength(count, result);
    for(auto& each : vs_) {
      seriValueType(each->getType(), result);
      each->valueSeri(result);
    }
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

  explicit Data(const std::vector<uint8_t>&);

  explicit Data(const char* ptr);

  explicit Data(std::string str);

  explicit Data(uint8_t num);

  explicit Data(uint16_t num);

  explicit Data(int num);

  explicit Data(uint32_t num);

  explicit Data(uint64_t num);

  explicit Data(bool b);

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
    std::string tmp;
    tmp.resize(length);
    memcpy(&(*tmp.begin()), &(data_.front()), length);
    result.append(tmp);
  }

  void setData(const std::vector<uint8_t>& datas) {
    data_ = datas;
  }

  void setData(const char* ptr) {
    data_.clear();
    size_t len = strlen(ptr);
    for(size_t i = 0; i < len; ++i) {
      data_.push_back(ptr[i]);
    }
  }

  void setData(std::string str) {
    size_t len = str.size();
    data_.resize(len);
    memcpy(&data_.front(), str.data(), len);
  }

  void setData(uint8_t num) {
    data_.clear();
    data_.resize(sizeof(num));
    memcpy(&data_.front(), &num, sizeof(num));
  }

  void setData(uint16_t num) {
    data_.clear();
    data_.resize(sizeof(num));
    memcpy(&data_.front(), &num, sizeof(num));
  }

  void setData(uint32_t num) {
    data_.clear();
    data_.resize(sizeof(num));
    memcpy(&data_.front(), &num, sizeof(num));
  }

  void setData(int num) {
    data_.clear();
    data_.resize(sizeof(num));
    memcpy(&data_.front(), &num, sizeof(num));
  }

  void setData(uint64_t num) {
    data_.clear();
    data_.resize(sizeof(num));
    memcpy(&data_.front(), &num, sizeof(num));
  }

  void setData(bool b) {
    char c;
    data_.resize(sizeof(c));
    if(b == true) {
      c = 1;
    }
    else {
      c = 0;
    }
    memcpy(&data_.front(), &c, sizeof(c));
  }

  std::vector<uint8_t> getBinary() {
    return data_;
  }

  std::string getString() {
    std::string result;
    result.resize(data_.size());
    for(size_t i = 0; i < data_.size(); ++i) {
      result[i] = data_[i];
    }
    return result;
  }

  uint8_t getUint8() {
    assert(data_.size() == sizeof(uint8_t));
    uint8_t tmp;
    memcpy(&tmp, &data_.front(), data_.size());
    return tmp;
  }

  uint16_t getUint16() {
    assert(data_.size() == sizeof(uint16_t));
    uint16_t tmp;
    memcpy(&tmp, &data_.front(), data_.size());
    return tmp;
  }

  uint32_t getUint32() {
    assert(data_.size() == sizeof(uint32_t));
    uint32_t tmp;
    memcpy(&tmp, &data_.front(), data_.size());
    return tmp;
  }

  int getInt() {
    assert(data_.size() == sizeof(int));
    int tmp;
    memcpy(&tmp, &data_.front(), data_.size());
    return tmp;
  }

  uint64_t getUint64() {
    assert(data_.size() == sizeof(uint64_t));
    uint64_t tmp;
    memcpy(&tmp, &data_.front(), data_.size());
    return tmp;
  }

  bool getBool() {
    char c;
    assert(data_.size() == sizeof(c));
    memcpy(&c, &data_.front(), data_.size());
    if(c == 1) {
      return true;
    }
    else {
      return false;
    }
  }

  template<typename T>
  T get();
};
}
