#include "elti/util.h"
#include "elti/value.h"
#include "elti/seri_parse.h"
#include "varint.h"
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>

namespace elti {
Map* getValueAsMap(Value* v) {
  assert(v->getType() == ValueType::Map);
  return static_cast<Map*>(v);
}

Array* getValueAsArray(Value* v) {
  assert(v->getType() == ValueType::Array);
  return static_cast<Array*>(v);
}

Data* getValueAsData(Value* v) {
  assert(v->getType() == ValueType::Data);
  return static_cast<Data*>(v);
}

DataRef* getValueAsDataRef(Value* v) {
  assert(v->getType() == ValueType::DataRef);
  return static_cast<DataRef*>(v);
}

void seri(const std::vector<uint8_t>& obj, std::vector<uint8_t>& container) {
  container = obj;
}

void seri(std::vector<uint8_t>&& obj, std::vector<uint8_t>& container) {
  container = std::move(obj);
}

void seri(const std::string& obj, std::vector<uint8_t>& container) {
  size_t len = obj.size();
  container.resize(len);
  memcpy(&container.front(), obj.data(), len);
}

void seri(const char* obj, std::vector<uint8_t>& container) {
  size_t len = strlen(obj);
  container.resize(len);
  memcpy(&container.front(), obj, len);
}

void seri(char* obj, std::vector<uint8_t>& container) {
  size_t len = strlen(obj);
  container.resize(len);
  memcpy(&container.front(), obj, len);
}

void seri(const int8_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const uint8_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const int16_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const uint16_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const int32_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const uint32_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const int64_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const uint64_t& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj));
  memcpy(&container.front(), &obj, sizeof(obj));
}

void seri(const varintNum& obj, std::vector<uint8_t>& container) {
  char buf[128];
  unsigned char bytes = 0;
  varint_encode(obj.getNum(), buf, sizeof(buf), &bytes);
  container.resize((size_t)bytes);
  memcpy(&container.front(), buf, bytes);
}

void seri(const bool& obj, std::vector<uint8_t>& container) {
  char c;
  container.resize(sizeof(c));
  if(obj == true) {
    c = 1;
  }
  else {
    c = 0;
  }
  memcpy(&container.front(), &c, sizeof(c));
}

template<>
std::string parse(const char* ptr, size_t n) {
  return std::string(ptr, n);
}

template<>
std::vector<uint8_t> parse(const char* ptr, size_t n) {
  std::vector<uint8_t> result(n);
  memcpy(&result.front(), ptr, n);
  return result;
}

template<>
int8_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(int8_t));
  int8_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
uint8_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(uint8_t));
  uint8_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
int16_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(int16_t));
  int16_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
uint16_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(uint16_t));
  uint16_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
int32_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(int32_t));
  int32_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
uint32_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(uint32_t));
  uint32_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
int64_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(int64_t));
  int64_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
uint64_t parse(const char* ptr, size_t n) {
  assert(n == sizeof(uint64_t));
  uint64_t result;
  memcpy(&result, ptr, n);
  return result;
}

template<>
varintNum parse(const char* ptr, size_t n) {
  unsigned long long tmp;
  unsigned char bytes;
  tmp = varint_decode(ptr, n, &bytes);
  assert(n == static_cast<size_t>(bytes));
  return varintNum(tmp);
}

template<>
bool parse(const char* ptr, size_t n) {
  char c;
  assert(n == sizeof(c));
  memcpy(&c, ptr, n);
  if (c != 0) {
    return true;
  }
  else {
    return false;
  }
}
}
