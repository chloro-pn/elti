#include "catch.hpp"
#include "elti/elti_enum.h"
#include <vector>
#include <iostream>

using namespace elti;
using namespace elti::detail;

#define HEX_TEST(x, str) \
  n = x; \
  r1 = to_json(ELTI_BYTES, (const char*)&n, sizeof(n), BytesEncode::HexStyle); \
  REQUIRE(r1 == "\"" str "\"")

TEST_CASE("json test", "[to_json]") {
  std::string bytes("hello");
  std::string r1 = to_json(ELTI_BYTES, bytes.data(), bytes.size(), BytesEncode::Base64Mime);
  REQUIRE(r1 == std::string("\"aGVsbG8=\""));

  std::vector<uint8_t> bytes2 = {'h', 'e', 'l', 'l', 'o'};
  r1 = to_json(ELTI_BYTES, (const char*)&*bytes2.begin(), bytes2.size(), BytesEncode::Base64Mime);
  REQUIRE(r1 == std::string("\"aGVsbG8=\""));

  std::string str("world");
  r1 = to_json(ELTI_STRING, str.data(), str.size(), BytesEncode::Base64Mime);
  REQUIRE(r1 == "\"world\"");

  uint32_t i = 9527;
  r1 = to_json(ELTI_UINT32, (const char*)&i, sizeof(i), BytesEncode::Base64Mime);
  REQUIRE(r1 == "9527");

  uint8_t n = 0xAB;
  r1 = to_json(ELTI_BYTES, (const char*)&n, sizeof(n), BytesEncode::HexStyle);
  REQUIRE(r1 == "\"AB\"");

  HEX_TEST(0x00, "00");
  HEX_TEST(0xFF, "FF");
  HEX_TEST(0x99, "99");
}
