#include "elti/value.h"
#include "elti/util.h"
#include "elti/factory.h"
#include <iterator> // std::advance.
#include <cstdlib>
#include <cstdio>

namespace elti {
Value::Value(ValueType type) : type_(type) {

}

void Value::valueParse(const char*& begin, size_t& offset, ParseRef ref) {
  if(type_ == ValueType::Map) {
    static_cast<Map*>(this)->valueParse(begin, offset, ref);
  }
  else if(type_ == ValueType::Array) {
    static_cast<Array*>(this)->valueParse(begin, offset, ref);
  }
  else if(type_ == ValueType::Data) {
    assert(ref == ParseRef::Off);
    static_cast<Data*>(this)->valueParse(begin, offset);
  }
  else if(type_ == ValueType::DataRef) {
    assert(ref == ParseRef::On);
    static_cast<DataRef*>(this)->valueParse(begin, offset);
  }
  else {
    fprintf(stderr, "valueParse error value type.");
    exit(-1);
  }
}

void Value::valueSeri(std::string& result) const {
  if(type_ == ValueType::Map) {
    static_cast<const Map*>(this)->valueSeri(result);
  }
  else if(type_ == ValueType::Array) {
    static_cast<const Array*>(this)->valueSeri(result);
  }
  else if(type_ == ValueType::Data) {
    static_cast<const Data*>(this)->valueSeri(result);
  }
  else if(type_ == ValueType::DataRef) {
    static_cast<const DataRef*>(this)->valueSeri(result);
  }
  else {
    fprintf(stderr, "valueSeri error value type.");
    exit(-1);
  }
}

std::string Value::toJson(const BytesEncode& be) const {
  if(type_ == ValueType::Map) {
    return static_cast<const Map*>(this)->toJson(be);
  }
  else if(type_ == ValueType::Array) {
    return static_cast<const Array*>(this)->toJson(be);
  }
  else if(type_ == ValueType::Data) {
    return static_cast<const Data*>(this)->toJson(be);
  }
  else if(type_ == ValueType::DataRef) {
    return static_cast<const DataRef*>(this)->toJson(be);
  }
  else {
    fprintf(stderr, "toJson error value type.");
    exit(-1);
  }
}

Map::Map() : Value(ValueType::Map) {

}

void Map::valueParse(const char*& begin, size_t& offset, ParseRef ref) {
  kvs_.clear();
  parseLength(begin, offset); //total size.
  uint64_t count = parseLength(begin, offset);
  for(uint64_t i = 0; i < count; ++i) {
    Element tmp;
    tmp.parse(begin, offset, ref);
    CHECK_PTR(begin);
    kvs_.push_back(std::move(tmp));
  }
}

void Map::valueSeri(std::string& result) const {
  std::string tmp;
  uint32_t count = kvs_.size();
  seriLength(count, tmp);
  for(auto& each : kvs_) {
    each.seri(tmp);
  }
  seriLength(tmp.size(), result);
  result.append(tmp);
}

std::string Map::toJson(const BytesEncode& be) const {
  std::string result;
  result.push_back('{');
  for (const auto& each : kvs_) {
    result.push_back('\"');
    result.append(each.getKey());
    result.append("\":");
    result.append(each.getValue()->toJson(be));
    result.push_back(',');
  }
  if(result.back() == ',') {
    result.back() = '}';
  }
  else {
    result.push_back('}');
  }
  return result;
}

void Map::set(const char* attr, std::unique_ptr<Value>&& v) {
  erase(attr);
  kvs_.push_back(Element(attr, std::move(v)));
}

void Map::erase(const char *attr) {
  auto it = std::find_if(kvs_.begin(), kvs_.end(), [attr](const Element& e)->bool {
    return strcmp(attr, e.getKey()) == 0;
  });
  if(it != kvs_.end()) {
    kvs_.erase(it);
  }
}

bool Map::contains(const char* attr) const {
  for(auto& each : kvs_) {
    if(strcmp(attr, each.getKey()) == 0) {
      return true;
    }
  }
  return false;
}

// inner func.
Value* Map::operator[](const char* attr) {
  for(auto& each : kvs_) {
    if(strcmp(attr, each.getKey()) == 0) {
      return each.v_.get();
    }
  }
  return nullptr;
}

Array::Array() : Value(ValueType::Array) {

}

void Array::valueParse(const char*& begin, size_t& offset, ParseRef ref) {
  vs_.clear();
  parseLength(begin, offset); //total size.
  uint64_t count = parseLength(begin, offset);
  for(uint64_t i = 0; i < count; ++i) {
    ValueType type = parseValueType(begin, offset);
    std::unique_ptr<Value> v = valueFactory(type, ref);
    v->valueParse(begin, offset, ref);
    vs_.push_back(std::move(v));
  }
}

void Array::valueSeri(std::string& result) const {
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

std::string Array::toJson(const BytesEncode& be) const {
  std::string result;
  result.push_back('[');
  for(const auto& each : vs_) {
    result.append(each->toJson(be));
    result.push_back(',');
  }
  if(result.back() == ',') {
    result.back() = ']';
  }
  else {
    result.push_back(']');
  }
  return result;
}

size_t Array::size() const {
  return vs_.size();
}

void Array::push_back(std::unique_ptr<Value>&& v) {
  vs_.push_back(std::move(v));
}

void Array::erase(size_t n) {
  if(n >= vs_.size()) {
    return;
  }
  auto it = vs_.begin();
  std::advance(it, n);
  vs_.erase(it);
}

Value* Array::operator[](size_t index) {
  if(index >= vs_.size()) {
    return nullptr;
  }
  return vs_[index].get();
}

Array::~Array() {

}

Data::Data() : Value(ValueType::Data), data_(), data_type_(ELTI_INVALID) {

}

// data := | total_length | data_type | ... bytes ... |
//             variant          1      total_length - 1
void Data::valueParse(const char*& begin, size_t& offset) {
  uint64_t size = parseLength(begin, offset);
  data_type_ = parseDataType(begin, offset);
  assert(size > 1);
  size = size - 1;
  data_.resize(size);
  memcpy(&(*data_.begin()), begin, size);
  begin += size;
  offset += size;
}

void Data::valueSeri(std::string& result) const {
  uint32_t length = data_.size() + 1;
  seriLength(length, result);
  seriDataType(data_type_, result);
  result.append(reinterpret_cast<const char*>(&data_.front()), data_.size());
}

//Data不携带类型信息，这里如何处理。
std::string Data::toJson(const BytesEncode& be) const {
  return detail::to_json(data_type_, reinterpret_cast<const char*>(&data_.front()), data_.size(), be);
}

const std::vector<uint8_t>& Data::bytesRef() const {
  return data_;
}

void DataRef::valueParse(const char *&begin, size_t &offset) {
  length_ = parseLength(begin, offset) - 1;
  data_type_ = parseDataType(begin, offset);
  ptr_ = begin;
  begin += length_;
  offset += length_;
}

void DataRef::valueSeri(std::string &result) const {
  uint32_t length = static_cast<uint32_t>(length_ + 1);
  seriLength(length, result);
  seriDataType(data_type_, result);
  result.append(ptr_, length_);
}

std::string DataRef::toJson(const BytesEncode& be) const {
  return detail::to_json(data_type_, ptr_, length_, be);
}

// DataRef的构造不走这个工厂。
std::unique_ptr<Value> valueFactory(ValueType type, ParseRef ref) {
  std::unique_ptr<Value> v_ = nullptr;
  if(type == ValueType::Map) {
    v_ = std::unique_ptr<Map>(new Map());
  }
  else if(type == ValueType::Array) {
    v_ = std::unique_ptr<Array>(new Array());
  }
  else if(type == ValueType::Data || type == ValueType::DataRef) {
    if(ref == ParseRef::Off)
      v_ = std::unique_ptr<Data>(new Data());
    else
      v_ = createRef(nullptr, 0);
  }
  else {
    fprintf(stderr, "valueFactory error.");
    exit(-1);
  }
  return v_;
}

std::unique_ptr<DataRef> createRef(const char* ptr, size_t len) {
  std::unique_ptr<DataRef> obj = std::unique_ptr<DataRef>(new DataRef());
  obj->setPtr(ptr);
  obj->setLength(len);
  return obj;
}
}
