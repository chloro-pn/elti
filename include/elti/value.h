#pragma once

#include "elti/util.h"
#include "elti/elti_type_trait.h"
#include "elti/seri_parse.h"
#include "elti/key.h"
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <type_traits>
#include <utility>

namespace elti {
class Value;
class Element {
  friend class Elti;
  friend class Map;

private:
  Key key_;
  std::unique_ptr<Value> v_;

  Value* getValue();
  const Value* getValue() const;

public:
  Element();
  Element(const char* ptr, std::unique_ptr<Value>&& v);
  Element(const Element&) = delete;
  Element& operator=(const Element&) = delete;
  Element(Element&& other) noexcept;
  Element& operator=(Element&& other) noexcept;

  const char* getKey() const {
    return key_.key_.c_str();
  }

  void parse(const char*& begin, size_t& offset, ParseRef ref);

  template<typename Outer>
  void seri(Outer& outer) const;

  ~Element();
};

class Value {
  friend class Element;
  friend class Map;
  friend class Array;
  friend class Data;
  friend class DataRef;
private:
  ValueType type_;

public:
  explicit Value(ValueType type);

  ValueType getType() const {
    return type_;
  }

  std::string toJson(const BytesEncode& be) const;
  virtual ~Value() = default;

protected:
  void valueParse(const char*& begin, size_t& offset, ParseRef ref);

  template<typename Outer>
  void valueSeri(Outer& outer) const;
};

class Map : public Value {
  friend class Value;
  friend class ValueWrapper;
protected:
  std::vector<Element> kvs_;
  Value* operator[](const char* attr);
  void valueParse(const char*& begin, size_t& offset, ParseRef ref);

  template<typename Outer>
  void valueSeri(Outer& outer) const {
    std::string tmp;
    uint32_t count = kvs_.size();
    seriLength(count, tmp);
    for(auto& each : kvs_) {
      each.seri(tmp);
    }
    seriLength(tmp.size(), outer);
    outer.append(tmp);
  }

public:
  Map();
  Map(Map&&) = default;
  Map& operator=(Map&&) = default;
  std::string toJson(const BytesEncode& be) const;
  void set(const char* attr, std::unique_ptr<Value>&& v);
  void erase(const char* attr);
  bool contains(const char* attr) const;
  ~Map() = default;
};

class Array : public Value {
  friend class Value;
  friend class ValueWrapper;
protected:
  std::vector<std::unique_ptr<Value>> vs_;
  void valueParse(const char*& begin, size_t& offset, ParseRef ref);

  template<typename Outer>
  void valueSeri(Outer& outer) const {
    std::string tmp;
    uint32_t count = vs_.size();
    seriLength(count, tmp);
    for(auto& each : vs_) {
      seriValueType(each->getType(), tmp);
      each->valueSeri(tmp);
    }
    seriLength(tmp.size(), outer);
    outer.append(tmp);
  }

  Value* operator[](size_t index);

public:
  Array();
  Array(Array&&) = default;
  Array& operator=(Array&&) = default;
  std::string toJson(const BytesEncode& be) const;
  size_t size() const;
  void push_back(std::unique_ptr<Value>&& v);
  void erase(size_t n);
  ~Array();
};

class Data : public Value {
  friend class Value;
  friend class ValueWrapper;
  friend std::unique_ptr<Value> valueFactory(ValueType type, ParseRef ref);
private:
  std::vector<uint8_t> data_;
  DataType data_type_;
  Data();
  void valueParse(const char*& begin, size_t& offset);

  template<typename Outer>
  void valueSeri(Outer& outer) const {
    uint32_t length = data_.size() + 1;
    seriLength(length, outer);
    seriDataType(data_type_, outer);
    outer.append(reinterpret_cast<const char*>(&data_.front()), data_.size());
  }

public:
  template<typename T>
  explicit Data(T&& obj) : Value(ValueType::Data) {
    data_type_ = detail::DataTypeTrait<T>::dt;
    seri(std::forward<T>(obj), data_);
  }

  Data(const Data&) = default;
  Data& operator=(const Data&) = default;

  template<typename T>
  Data& operator=(T&& other) {
    data_type_ = detail::DataTypeTrait<T>::dt;
    seri(std::forward<T>(other), data_);
    return *this;
  }

  std::string toJson(const BytesEncode& be) const;
  const std::vector<uint8_t>& bytesRef() const;

  template<typename T>
  T get() const {
    static_assert (detail::NoRefNoPointer<T>::value, "get() method should not return & or * type"
                                                     "(including <T* const> and <const T*> type).\n");
    assert(detail::DataTypeTrait<T>::dt == data_type_);
    return parse<T>(reinterpret_cast<const char*>(&*data_.begin()), data_.size());
  }

  ~Data() = default;
};

class DataRef : public Value {
  friend class Value;
  friend class ValueWrapper;
  friend std::unique_ptr<DataRef> createRef(const char* ptr, size_t len);
 private:
  const char* ptr_;
  size_t length_;
  DataType data_type_;

  DataRef():Value(ValueType::DataRef), ptr_(nullptr), length_(0), data_type_(ELTI_INVALID) {}

  DataRef(const char* ptr, size_t length):Value(ValueType::DataRef),
                                          ptr_(ptr),
                                          length_(length),
                                          data_type_(ELTI_INVALID) {

  }

  void valueParse(const char*& begin, size_t& offset);

  template<typename Outer>
  void valueSeri(Outer& outer) const {
    uint32_t length = static_cast<uint32_t>(length_ + 1);
    seriLength(length, outer);
    seriDataType(data_type_, outer);
    outer.append(ptr_, length_);
  }

  void setPtr(const char* ptr) { ptr_ = ptr; }
  void setDataType(const DataType& dt) { data_type_ = dt; }
  void setLength(size_t length) { length_ = length; }

public:
  std::string toJson(const BytesEncode& be) const;

  template<typename T>
  T get() const {
    static_assert (detail::NoRefNoPointer<T>::value, "get() method should not return & or * type"
                                                     "(including <T* const> and <const T*> type).\n");
    assert(detail::DataTypeTrait<T>::dt == data_type_);
    return parse<T>(ptr_, length_);
  }

  const char* getPtr() const { return ptr_; }
  size_t getLength() const { return length_; }
  ~DataRef() = default;
};

template<typename Outer>
void Element::seri(Outer& outer) const {
  key_.keySeri(outer);
  seriValueType(v_->getType(), outer);
  v_->valueSeri(outer);
}

template<typename Outer>
void Value::valueSeri(Outer& outer) const {
  if(type_ == ValueType::Map) {
    static_cast<const Map*>(this)->valueSeri(outer);
  }
  else if(type_ == ValueType::Array) {
    static_cast<const Array*>(this)->valueSeri(outer);
  }
  else if(type_ == ValueType::Data) {
    static_cast<const Data*>(this)->valueSeri(outer);
  }
  else if(type_ == ValueType::DataRef) {
    static_cast<const DataRef*>(this)->valueSeri(outer);
  }
  else {
    fprintf(stderr, "valueSeri error value type.");
    exit(-1);
  }
}

std::unique_ptr<Value> valueFactory(ValueType type, ParseRef ref);

std::unique_ptr<DataRef> createRef(const char* ptr, size_t len);
}
