#pragma once

#include "elti/elti_enum.h"
#include "varint.h"
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#define MAP_TYPE 0
#define ARRAY_TYPE 1
#define DATA_TYPE 2

namespace elti {
  class Value;
  class Map;
  class Array;
  class Data;
  class DataRef;

  ValueType parseValueType(const char*& begin, size_t& offset);

  DataType parseDataType(const char*& begin, size_t& offset);

  uint64_t parseLength(const char*& begin, size_t& offset);

  template<typename Outer>
  void seriValueType(ValueType type, Outer& outer) {
    char tmp;
    if(type == ValueType::Map) {
      tmp = MAP_TYPE;
    }
    else if(type == ValueType::Array) {
      tmp = ARRAY_TYPE;
    }
    // 将DataRef处理为Data类型。
    else if(type == ValueType::Data || type == ValueType::DataRef) {
      tmp = DATA_TYPE;
    }
    else {
      fprintf(stderr, "seriValueType error.");
      exit(-1);
    }
    outer.push_back(tmp);
  }
  
  template<typename Outer>
  void seriDataType(DataType dt, Outer& outer) {
    outer.push_back(static_cast<char>(dt));
  }

  template<typename Outer>
  void seriLength(uint32_t length, Outer& outer) {
    char buf[128];
    unsigned char bytes = 0;
    varint_encode(length, buf, sizeof(buf), &bytes);
    outer.append(static_cast<const char*>(buf), static_cast<size_t>(bytes));
  }

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
