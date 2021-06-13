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
std::unique_ptr<Value> valueFactory(ValueType type, ParseRef ref);
std::unique_ptr<DataRef> createRef(const char* ptr, size_t len);

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

  template<typename Inner>
  void parse(const Inner& inner, size_t& offset, ParseRef ref);

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
  template<typename Inner>
  void valueParse(const Inner& inner, size_t& offset, ParseRef ref);

  template<typename Outer>
  void valueSeri(Outer& outer) const;
};

class Map : public Value {
  friend class Value;
  friend class ValueWrapper;
protected:
  std::vector<Element> kvs_;
  Value* operator[](const char* attr);

  template<typename Inner>
  void valueParse(const Inner& inner, size_t& offset, ParseRef ref) {
    kvs_.clear();
    parseLength(inner, offset); //total size.
    uint64_t count = parseLength(inner, offset);
    for(uint64_t i = 0; i < count; ++i) {
      Element tmp;
      tmp.parse(inner, offset, ref);
      kvs_.push_back(std::move(tmp));
    }
  }

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

  template<typename Inner>
  void valueParse(const Inner& inner, size_t& offset, ParseRef ref) {
    vs_.clear();
    parseLength(inner, offset); //total size.
    uint64_t count = parseLength(inner, offset);
    for(uint64_t i = 0; i < count; ++i) {
      ValueType type = parseValueType(inner, offset);
      std::unique_ptr<Value> v = valueFactory(type, ref);
      v->valueParse(inner, offset, ref);
      vs_.push_back(std::move(v));
    }
  }

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

  // data := | total_length | data_type | ... bytes ... |
  //             variant          1      total_length - 1
  template<typename Inner>
  void valueParse(const Inner& inner, size_t& offset) {
    uint64_t size = parseLength(inner, offset);
    data_type_ = parseDataType(inner, offset);
    assert(size > 1);
    size = size - 1;
    data_.resize(size);
    memcpy(&(*data_.begin()), inner.curAddr(), size);
    inner.skip(size);
    offset += size;
  }

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

  template<typename Inner>
  void valueParse(const Inner& inner, size_t &offset) {
    length_ = parseLength(inner, offset) - 1;
    data_type_ = parseDataType(inner, offset);
    ptr_ = inner.curAddr();
    inner.skip(length_);
    offset += length_;
  }

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

template<typename Inner>
void Element::parse(const Inner& inner, size_t& offset, ParseRef ref) {
  key_.keyParse(inner, offset);
  ValueType type = parseValueType(inner, offset);
  v_ = valueFactory(type, ref);
  v_->valueParse(inner, offset, ref);
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

template<typename Inner>
void Value::valueParse(const Inner& inner, size_t& offset, ParseRef ref) {
  if(type_ == ValueType::Map) {
    static_cast<Map*>(this)->valueParse(inner, offset, ref);
  }
  else if(type_ == ValueType::Array) {
    static_cast<Array*>(this)->valueParse(inner, offset, ref);
  }
  else if(type_ == ValueType::Data) {
    assert(ref == ParseRef::Off);
    static_cast<Data*>(this)->valueParse(inner, offset);
  }
  else if(type_ == ValueType::DataRef) {
    assert(ref == ParseRef::On);
    static_cast<DataRef*>(this)->valueParse(inner, offset);
  }
  else {
    fprintf(stderr, "valueParse error value type.");
    exit(-1);
  }
}
}
