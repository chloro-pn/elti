#include "catch.hpp"
#include "elti/seri_parse.h"
#include "elti/varint_num.h"
#include <vector>
#include <cstdint>

using namespace elti;

TEST_CASE("seri and parse test", "[seri and parse]") {
  std::vector<uint8_t> container;
  SECTION("std::string") {
    std::string str("hello world");
    seri(str, container);
    REQUIRE(str.size() == container.size());
    for(size_t i = 0; i < str.size(); ++i) {
      REQUIRE(str[i] == static_cast<char>(container[i]));
    }
    REQUIRE(parse<std::string>(container) == str);
  }
  SECTION("binary") {
    std::vector<uint8_t> bin = {0x01, 0xd4, 0xc5};
    seri(bin, container);
    REQUIRE(bin.size() == container.size());
    for(size_t i = 0; i < bin.size(); ++i) {
      REQUIRE(bin[i] == container[i]);
    }
    REQUIRE(parse<std::vector<uint8_t>>(container) == bin);
  }
  SECTION("integer") {
    int8_t num1 = 23;
    seri(num1, container);
    REQUIRE(container.size() == sizeof(int8_t));
    REQUIRE(parse<int8_t>(container) == num1);
    container.clear();

    uint8_t num2 = 1;
    seri(num2, container);
    REQUIRE(container.size() == sizeof(uint8_t));
    REQUIRE(parse<uint8_t>(container) == num2);
    container.clear();

    int16_t num3 = 21;
    seri(num3, container);
    REQUIRE(container.size() == sizeof(int16_t));
    REQUIRE(parse<int16_t>(container) == num3);
    container.clear();

    uint16_t num4 = 3;
    seri(num4, container);
    REQUIRE(container.size() == sizeof(uint16_t));
    REQUIRE(parse<uint16_t>(container) == num4);
    container.clear();

    int32_t num5 = 13434;
    seri(num5, container);
    REQUIRE(container.size() == sizeof(int32_t));
    REQUIRE(parse<int32_t>(container) == num5);
    container.clear();

    uint32_t num6 = 134535;
    seri(num6, container);
    REQUIRE(container.size() == sizeof(uint32_t));
    REQUIRE(parse<uint32_t>(container) == num6);
    container.clear();

    int64_t num7 = 2313;
    seri(num7, container);
    REQUIRE(container.size() == sizeof(int64_t));
    REQUIRE(parse<int64_t>(container) == num7);
    container.clear();

    uint64_t num8 = 112434;
    seri(num8, container);
    REQUIRE(container.size() == sizeof(uint64_t));
    REQUIRE(parse<uint64_t>(container) == num8);
    container.clear();
  }
  SECTION("varint") {
    varintNum num(123);
    seri(num, container);
    REQUIRE(container.size() == 1);
    REQUIRE(parse<varintNum>(container).getNum() == 123);
    varintNum num2(129);
    container.clear();

    seri(num2, container);
    REQUIRE(container.size() == 2);
    REQUIRE(parse<varintNum>(container).getNum() == 129);
  }
  SECTION("bool") {
    bool t = true;
    seri(t, container);
    REQUIRE(container.size() == 1);
    REQUIRE(parse<bool>(container) == t);
  }
}
