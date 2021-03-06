#include "elti/value.h"
#include "elti/util.h"
#include <iterator> // std::advance.
#include <cstdlib>
#include <cstdio>

namespace elti {
Value::Value(ValueType type) : type_(type) {

}

void Value::valueParse(const char*& begin, size_t& offset) {
  if(type_ == ValueType::Map) {
    static_cast<Map*>(this)->valueParse(begin, offset);
  }
  else if(type_ == ValueType::Array) {
    static_cast<Array*>(this)->valueParse(begin, offset);
  }
  else if(type_ == ValueType::Data) {
    static_cast<Data*>(this)->valueParse(begin, offset);
  }
  else {
    fprintf(stderr, "valueParse error value type.");
    exit(-1);
  }
}

void Value::valueSeri(std::string& result) {
  if(type_ == ValueType::Map) {
    static_cast<Map*>(this)->valueSeri(result);
  }
  else if(type_ == ValueType::Array) {
    static_cast<Array*>(this)->valueSeri(result);
  }
  else if(type_ == ValueType::Data) {
    static_cast<Data*>(this)->valueSeri(result);
  }
  else {
    fprintf(stderr, "valueSeri error value type.");
    exit(-1);
  }
}

Map::Map() : Value(ValueType::Map) {

}

void Map::valueParse(const char*& begin, size_t& offset) {
  parseLength(begin, offset); //total size.
  uint64_t count = parseLength(begin, offset);
  for(uint64_t i = 0; i < count; ++i) {
    Element tmp;
    tmp.parse(begin, offset);
    CHECK_PTR(begin);
    kvs_.push_back(std::move(tmp));
  }
}

void Map::valueSeri(std::string& result) {
  std::string tmp;
  uint32_t count = kvs_.size();
  seriLength(count, tmp);
  for(auto& each : kvs_) {
    each.seri(tmp);
  }
  seriLength(tmp.size(), result);
  result.append(tmp);
}

void Map::set(const char* attr, Value* v) {
  erase(attr);
  kvs_.push_back(Element(attr, v));
}

void Map::erase(const char *attr) {
  auto it = std::find_if(kvs_.begin(), kvs_.end(), [attr](const Element& e)->bool {
    return strcmp(attr, e.getKey()) == 0;
  });
  if(it != kvs_.end()) {
    kvs_.erase(it);
  }
}

bool Map::contains(const char* attr) {
  for(auto& each : kvs_) {
    if(strcmp(attr, each.getKey()) == 0) {
      return true;
    }
  }
  return false;
}

Value* Map::operator[](const char* attr) {
  for(auto& each : kvs_) {
    if(strcmp(attr, each.getKey()) == 0) {
      return each.v_;
    }
  }
  return nullptr;
}

Array::Array() : Value(ValueType::Array) {

}

void Array::valueParse(const char*& begin, size_t& offset) {
  parseLength(begin, offset); //total size.
  uint64_t count = parseLength(begin, offset);
  for(uint64_t i = 0; i < count; ++i) {
    ValueType type = parseValueType(begin, offset);
    Value* v = valueFactory(type);
    v->valueParse(begin, offset);
    vs_.push_back(v);
  }
}

void Array::valueSeri(std::string& result) {
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

uint64_t Array::size() {
  return vs_.size();
}

void Array::push_back(Value* v) {
  vs_.push_back(v);
}

void Array::erase(size_t n) {
  assert(n < vs_.size());
  auto it = vs_.begin();
  std::advance(it, n);
  cleanValue(*it);
  vs_.erase(it);
}

Value* Array::operator[](size_t index) {
  if(index >= vs_.size()) {
    return nullptr;
  }
  return vs_[index];
}

Array::~Array() {
  for(auto& each_value_ptr : vs_) {
    cleanValue(each_value_ptr);
  }
}

Data::Data() : Value(ValueType::Data), ptr_(nullptr), length_(0), type_(type::INVALID) {

}

Data::Data(const ref& obj) : Value(ValueType::Data), ptr_(obj.get_addr()), length_(obj.get_length()), type_(type::REF) {

}

Data& Data::operator=(const ref& obj) {
  type_ = type::REF;
  ptr_ = obj.get_addr();
  length_ = obj.get_length();
  return *this;
}

void Data::valueParse(const char*& begin, size_t& offset) {
  uint64_t size = parseLength(begin, offset);
  data_.resize(size);
  memcpy(&(*data_.begin()), begin, size);
  begin += size;
  offset += size;
  type_ = type::DATA;
}

void Data::valueSeri(std::string& result) {
  if (type_ == type::DATA) {
    uint32_t length = data_.size();
    seriLength(length, result);
    size_t old_len = result.size();
    result.resize(result.size() + length);
    memcpy(&(*(result.begin() + old_len)), &data_.front(), length);
  }
  else if (type_ == type::REF) {
    seriLength(static_cast<uint64_t>(length_), result);
    size_t old_len = result.size();
    result.resize(result.size() + length_);
    memcpy(&(*(result.begin() + old_len)), ptr_, length_);
  }
  else {
    fprintf(stderr, "Data::valueSeri error.");
    exit(-1);
  }
}

size_t Data::useBytes() const {
  return data_.size();
}

std::vector<uint8_t>& Data::bytesRef() {
  return data_;
}


Value* valueFactory(ValueType type) {
  Value* v_ = nullptr;
  if(type == ValueType::Map) {
    v_ = new Map();
  }
  else if(type == ValueType::Array) {
    v_ = new Array();
  }
  else if(type == ValueType::Data) {
    v_ = new Data();
  }
  else {
    fprintf(stderr, "valueFactory error.");
    exit(-1);
  }
  return v_;
}
}
