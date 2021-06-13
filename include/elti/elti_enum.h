#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <type_traits>
#include "elti/varint_num.h"

#define ELTI_BYTES  0x00
#define ELTI_STRING 0x01
#define ELTI_INT8   0x02
#define ELTI_UINT8  0x03
#define ELTI_INT16  0x04
#define ELTI_UINT16 0x05
#define ELTI_INT32  0x06
#define ELTI_UINT32 0x07
#define ELTI_INT64  0x08
#define ELTI_UINT64 0x09
#define ELTI_VARINT 0x0A
#define ELTI_BOOL   0x0B
#define ELTI_CUSTOM 0x0C
#define ELTI_INVALID 0x0D

#define DataType uint8_t

namespace elti {
enum class ValueType { Map, Array, Data, DataRef, Invalid };
enum class ParseRef { On, Off };
enum class BytesEncode { Base64Mime,  HexStyle };

namespace detail {
  template<typename T>
  struct InnerDataTypeTrait {
    static constexpr uint8_t dt = ELTI_CUSTOM;
  };

  //类模板的静态数据成员需要在头文件中定义
  template<typename T>
  constexpr DataType InnerDataTypeTrait<T>::dt;

#define DataTypeTraitStruct(x, y) \
template<> \
struct InnerDataTypeTrait<x> { \
  static constexpr DataType dt = ELTI_##y; \
};

  DataTypeTraitStruct(      char*,         STRING)
  DataTypeTraitStruct(const char*,         STRING)
  DataTypeTraitStruct(std::string,         STRING)
  DataTypeTraitStruct(std::vector<uint8_t>, BYTES)
  DataTypeTraitStruct(    int8_t,            INT8)
  DataTypeTraitStruct(   uint8_t,           UINT8)
  DataTypeTraitStruct(   int16_t,           INT16)
  DataTypeTraitStruct(  uint16_t,          UINT16)
  DataTypeTraitStruct(   int32_t,           INT32)
  DataTypeTraitStruct(  uint32_t,          UINT32)
  DataTypeTraitStruct(   int64_t,           INT64)
  DataTypeTraitStruct(  uint64_t,          UINT64)
  DataTypeTraitStruct( varintNum,          VARINT)
  DataTypeTraitStruct(      bool,            BOOL)

  // fixed stupid bug :(
  template<typename T>
  struct DataTypeTrait {
    static constexpr DataType dt = InnerDataTypeTrait<typename std::decay<T>::type>::dt;
  };

  template<typename T>
  constexpr DataType DataTypeTrait<T>::dt;
  
  template<size_t n>
  struct DataTypeTrait<char(&)[n]> {
    static constexpr DataType dt = ELTI_STRING;
  };

  template<size_t n>
  struct DataTypeTrait<const char(&)[n]> {
    static constexpr DataType dt = ELTI_STRING;
  };
  std::string hex_encode(const char* ptr, size_t length);
  std::string to_json(uint8_t DATA_TYPE, const char* ptr, size_t length, BytesEncode be);
}
}
