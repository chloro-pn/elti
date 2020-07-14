#include "elti/util.h"
#include "elti/value.h"
#include "varint.h"
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>

#define MAP_TYPE 0
#define ARRAY_TYPE 1
#define DATA_TYPE 2

namespace elti {
uint64_t parseLength(const char*& begin, size_t& offset) {
  if(false) {
    uint32_t tmp;
    memcpy(&tmp, begin, sizeof(tmp));
    begin += sizeof(tmp);
    offset += sizeof(tmp);
    return tmp;
  }
  else {
    unsigned long long tmp;
    unsigned char bytes;
    tmp = varint_decode((char*)begin, 128, &bytes);
    begin += bytes;
    offset += bytes;
    return tmp;
  }
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
  else {
    tmp = DATA_TYPE;
  }
  std::string t2(&tmp, sizeof(tmp));
  result.append(t2);
}

void seriLength(uint32_t length, std::string& result) {
  if(false) {
    char buf[sizeof(uint32_t)];
    memcpy(buf, &length, sizeof(length));
    result.append(buf, sizeof(uint32_t));
  }
  else {
    char buf[128];
    unsigned char bytes = 0;
    varint_encode(length, buf, sizeof(buf), &bytes);
    result.append(buf, static_cast<uint32_t>(bytes));
  }
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
}
