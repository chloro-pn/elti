#pragma once
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace elti {
  class Value;
  class Map;
  class Array;
  class Data;

  enum class ValueType { Map, Array, Data };

  ValueType parseValueType(const char*& begin, size_t& offset);

  uint64_t parseLength(const char*& begin, size_t& offset);

  void seriValueType(ValueType type, std::string& result);

  void seriLength(uint32_t length, std::string& result);

  Map* getValueAsMap(Value* v);

  Array* getValueAsArray(Value* v);

  Data* getValueAsData(Value* v);

  class varintNum {
    uint64_t num_;

  public:
    explicit varintNum(uint64_t num) : num_(num) {

    }

    uint64_t getNum() const {
      return num_;
    }
  };

  template<typename T>
  void seri(const T& obj, std::vector<uint8_t>& container);

  template<typename T>
  T parse(const std::vector<uint8_t>& container);

#define CHECK_PTR(x) \
if((x) == nullptr) {\
  fprintf(stderr, "parse error."); \
  exit(-1); \
}
}
