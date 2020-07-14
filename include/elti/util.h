#pragma once
#include <cstdint>
#include <cstdio>
#include <string>

class Value;
class Map;
class Array;
class Data;

namespace elti {

  enum class ValueType { Map, Array, Data };

  ValueType parseValueType(const char*& begin, size_t& offset);

  uint64_t parseLength(const char*& begin, size_t& offset);

  void seriValueType(ValueType type, std::string& result);

  void seriLength(uint32_t length, std::string& result);

  Map* getValueAsMap(Value* v);

  Array* getValueAsArray(Value* v);

  Data* getValueAsData(Value* v);

#define CHECK_PTR(x) \
if((x) == nullptr) {\
  fprintf(stderr, "parse error."); \
  exit(-1); \
}
}
