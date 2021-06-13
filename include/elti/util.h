#pragma once

#include "elti_enum.h"
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace elti {
  class Value;
  class Map;
  class Array;
  class Data;
  class DataRef;

  ValueType parseValueType(const char*& begin, size_t& offset);

  DataType parseDataType(const char*& begin, size_t& offset);

  uint64_t parseLength(const char*& begin, size_t& offset);

  void seriValueType(ValueType type, std::string& result);
  
  void seriDataType(DataType dt, std::string& result);

  void seriLength(uint32_t length, std::string& result);

  Map* getValueAsMap(Value* v);

  Array* getValueAsArray(Value* v);

  Data* getValueAsData(Value* v);

  DataRef* getValueAsDataRef(Value* v);

#define CHECK_PTR(x) \
if((x) == nullptr) {\
  fprintf(stderr, "parse error."); \
  exit(-1); \
}
}
