#include "catch.hpp"
#include "elti/seri_parse.h"
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
}
