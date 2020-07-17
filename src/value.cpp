#include "elti/value.h"

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
  else {
    static_cast<Data*>(this)->valueParse(begin, offset);
  }
}

void Value::valueSeri(std::string& result) {
  if(type_ == ValueType::Map) {
    static_cast<Map*>(this)->valueSeri(result);
  }
  else if(type_ == ValueType::Array) {
    static_cast<Array*>(this)->valueSeri(result);
  }
  else {
    static_cast<Data*>(this)->valueSeri(result);
  }
}

Map::Map() : Value(ValueType::Map) {

}

Array::Array() : Value(ValueType::Array) {

}

Data::Data() : Value(ValueType::Data) {

}

Value* valueFactory(ValueType type) {
  Value* v_ = nullptr;
  if(type == ValueType::Map) {
    v_ = new Map();
  }
  else if(type == ValueType::Array) {
    v_ = new Array();
  }
  else {
    v_ = new Data();
  }
  return v_;
}
}
