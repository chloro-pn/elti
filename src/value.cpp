#include "elti/value.h"
#include "elti/util.h"
#include "elti/factory.h"
#include <iterator> // std::advance.
#include <cstdlib>
#include <cstdio>

namespace elti {
Value::Value(ValueType type) : type_(type) {

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

//Data不携带类型信息，这里如何处理。
std::string Data::toJson(const BytesEncode& be) const {
  return detail::to_json(data_type_, reinterpret_cast<const char*>(&data_.front()), data_.size(), be);
}

const std::vector<uint8_t>& Data::bytesRef() const {
  return data_;
}

std::string DataRef::toJson(const BytesEncode& be) const {
  return detail::to_json(data_type_, ptr_, length_, be);
}

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
