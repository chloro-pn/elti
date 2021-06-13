#include "elti/elti_enum.h"
#include "elti/seri_parse.h"
#include "elti/varint_num.h"
#include "base64.h"
#include <cassert>
#include <string>
#include <vector>

namespace elti {
// 将后4位二进制数据转化为'0'~'F'的字符。
static char uint8_to_char(uint8_t n) {
  assert(n <= 0x0F);
  switch (n) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
    default:
      fprintf(stderr, "uint8_to_char error, invalid n : %d\n", n);
      exit(-1);
  }
}

namespace detail {
std::string hex_encode(const char* ptr, size_t length) {
  std::string result;
  result.reserve(128);
  for(size_t i = 0; i < length; ++i) {
    uint8_t byte = static_cast<uint8_t>(*(ptr + i));
    result.push_back(uint8_to_char(byte >> 4));
    result.push_back(uint8_to_char(byte & 0x0F));
  }
  return result;
}

#define PTR_TO_INTEGRAL(type, ptr) *reinterpret_cast<const type*>(ptr)
#define CASE_RETURN(type_tag, type, ptr) \
  case type_tag: return std::to_string(PTR_TO_INTEGRAL(type, ptr));

std::string to_json(uint8_t DATA_TYPE, const char* ptr, size_t length, BytesEncode be) {
  switch (DATA_TYPE) {
    case ELTI_CUSTOM:
    case ELTI_BYTES: {
      if(be == BytesEncode::Base64Mime) {
        return "\"" + base64_encode_mime(std::string(ptr, length)) + "\"";
      }
      else if(be == BytesEncode::HexStyle) {
        return "\"" + hex_encode(ptr, length) + "\"";
      }
      else {
        fprintf(stderr, "invalid bytesencode.\n");
        exit(-1);
      }
    }
    case ELTI_STRING:
      return "\"" + std::string(ptr, length) + "\"";
    CASE_RETURN(ELTI_INT8  , int8_t  , ptr)
    CASE_RETURN(ELTI_UINT8 , uint8_t , ptr)
    CASE_RETURN(ELTI_INT16 , int16_t , ptr)
    CASE_RETURN(ELTI_UINT16, uint16_t, ptr)
    CASE_RETURN(ELTI_INT32 , int32_t , ptr)
    CASE_RETURN(ELTI_UINT32, uint32_t, ptr)
    CASE_RETURN(ELTI_INT64 , int64_t , ptr)
    CASE_RETURN(ELTI_UINT64, uint64_t, ptr)
    case ELTI_VARINT: {
      varintNum vn = parse<varintNum>(ptr, length);
      return std::to_string(vn.getNum());
    }
    case ELTI_BOOL: {
      bool tmp = *reinterpret_cast<const bool*>(ptr);
      if(tmp == true)
        return "true";
      else
        return "false";
    }
    default:
      fprintf(stderr, "unknown data type : %d \n", DATA_TYPE);
      exit(-1);
  }
}
}
} // namespace elti
