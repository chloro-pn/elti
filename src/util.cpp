#include "elti/util.h"
#include "elti/value.h"
#include "varint.h"
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstdio>

#define MAP_TYPE 0
#define ARRAY_TYPE 1
#define DATA_TYPE 2

namespace elti {
uint64_t parseLength(const char*& begin, size_t& offset) {
  unsigned long long tmp;
  unsigned char bytes;
  tmp = varint_decode((char*)begin, 128, &bytes);
  begin += bytes;
  offset += bytes;
  return tmp;
}

ValueType parseValueType(const char*& begin, size_t& offset) {
  char tmp;
  memcpy(&tmp, begin, sizeof(tmp));
  begin += sizeof(tmp);
  offset += sizeof(tmp);
  if(tmp == MAP_TYPE) {
    return ValueType::Map;
  }
  else if(tmp == ARRAY_TYPE) {
    return ValueType::Array;
  }
  else if(tmp == DATA_TYPE) {
    return ValueType::Data;
  }
  else {
    fprintf(stderr, "error type : %d\n", tmp);
    exit(-1);
  }
}

void seriValueType(ValueType type, std::string& result) {
  char tmp;
  if(type == ValueType::Map) {
    tmp = MAP_TYPE;
  }
  else if(type == ValueType::Array) {
    tmp = ARRAY_TYPE;
  }
  else if(type == ValueType::Data) {
    tmp = DATA_TYPE;
  }
  else {
    fprintf(stderr, "seriValueType error.");
    exit(-1);
  }
  std::string t2(&tmp, sizeof(tmp));
  result.append(t2);
}

void seriLength(uint32_t length, std::string& result) {
  char buf[128];
  unsigned char bytes = 0;
  varint_encode(length, buf, sizeof(buf), &bytes);
  result.append(buf, static_cast<size_t>(bytes));
}

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

void cleanValue(Value* v) {
  if(v == nullptr) {
    return;
  }
  if(v->getType() == ValueType::Map) {
    delete getValueAsMap(v);
  }
  else if(v->getType() == ValueType::Array) {
    delete getValueAsArray(v);
  }
  else if(v->getType() == ValueType::Data) {
    delete getValueAsData(v);
  }
  else {
    fprintf(stderr, "cleanValue error.");
    exit(-1);
  }
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
std::string parse(const std::vector<uint8_t>& container) {
  std::string result;
  result.resize(container.size());
  memcpy(&result.front(), &container.front(), container.size());
  return result;
}

template<>
std::vector<uint8_t> parse(const std::vector<uint8_t>& container) {
  return container;
}

template<>
int8_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(int8_t));
  int8_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
uint8_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(uint8_t));
  uint8_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
int16_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(int16_t));
  int16_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
uint16_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(uint16_t));
  uint16_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
int32_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(int32_t));
  int32_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
uint32_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(uint32_t));
  uint32_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
int64_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(int64_t));
  int64_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
uint64_t parse(const std::vector<uint8_t>& container) {
  assert(container.size() == sizeof(uint64_t));
  uint64_t tmp;
  memcpy(&tmp, &container.front(), container.size());
  return tmp;
}

template<>
varintNum parse(const std::vector<uint8_t>& container) {
  unsigned long long tmp;
  unsigned char bytes;
  tmp = varint_decode((char*)&container.front(), container.size(), &bytes);
  assert(container.size() == static_cast<size_t>(bytes));
  return varintNum(tmp);
}

template<>
bool parse(const std::vector<uint8_t>& container) {
  char c;
  assert(container.size() == sizeof(c));
  memcpy(&c, &container.front(), container.size());
  if(c != 0) {
    return true;
  }
  else {
    return false;
  }
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
