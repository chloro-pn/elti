#pragma once

#include "elti/util.h"
#include "elti/element.h"
#include "elti/elti_type_trait.h"
#include "elti/seri_parse.h"
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <type_traits>
#include <utility>

namespace elti {
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
  void valueSeri(std::string& result) const;
};

class Map : public Value {
  friend class Value;
  friend class ValueWrapper;
protected:
  std::vector<Element> kvs_;
  Value* operator[](const char* attr);
  void valueParse(const char*& begin, size_t& offset, ParseRef ref);
  void valueSeri(std::string& result) const;

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
  void valueSeri(std::string& result) const;
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
  void valueSeri(std::string& result) const;

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
  void valueSeri(std::string& result) const;
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

std::unique_ptr<Value> valueFactory(ValueType type, ParseRef ref);

std::unique_ptr<DataRef> createRef(const char* ptr, size_t len);
}
