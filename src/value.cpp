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

Data::Data(const std::vector<uint8_t>& data) : Value(ValueType::Data), data_(data) {

}

Data::Data(const char* ptr) : Value(ValueType::Data) {
  setData(ptr);
}

Data::Data(std::string str) : Value(ValueType::Data) {
  setData(str);
}

Data::Data(uint8_t num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(uint16_t num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(uint32_t num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(uint64_t num) : Value(ValueType::Data) {
  setData(num);
}


Data::Data(int8_t num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(int16_t num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(int32_t num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(int64_t num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(varintNum num) : Value(ValueType::Data) {
  setData(num);
}

Data::Data(bool b) : Value(ValueType::Data) {
  setData(b);
}

template<>
std::string Data::get() {
  return getString();
}

template<>
std::vector<uint8_t> Data::get() {
  return getBinary();
}

template<>
std::uint8_t Data::get() {
  return getUint8();
}

template<>
std::uint16_t Data::get() {
  return getUint16();
}

template<>
std::uint32_t Data::get() {
  return getUint32();
}

template<>
std::uint64_t Data::get() {
  return getUint64();
}

template<>
std::int8_t Data::get() {
  return getint8();
}

template<>
std::int16_t Data::get() {
  return getint16();
}

template<>
std::int32_t Data::get() {
  return getint32();
}

template<>
std::int64_t Data::get() {
  return getint64();
}

template<>
varintNum Data::get() {
  return getVarintNum();
}

template<>
bool Data::get() {
  return getBool();
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
