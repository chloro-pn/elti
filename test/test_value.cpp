#include "catch.hpp"
#define private public
#define protected public
#include "elti/value.h"
#include "elti/factory.h"
#include "elti/elti.h"
#include <string>
#include <vector>

using namespace elti;

TEST_CASE("data test", "[value]") {
  Data data;
  REQUIRE(data.getType() == ValueType::Data);
  data = std::string("hello");
  REQUIRE(data.get<std::string>() == std::string("hello"));
  std::string result;
  data.valueSeri(result);
  size_t offset = 0;
  const char* ptr = result.data();
  data.valueParse(ptr, offset);
  REQUIRE(offset == result.size());
  REQUIRE(data.get<std::string>() == "hello");

  data = (uint32_t)2345;
  REQUIRE(data.get<uint32_t>() == 2345);

  data = true;
  REQUIRE(data.get<bool>() == true);

  auto tmp = std::vector<uint8_t>{0x01, 0x2a, 0xcd};
  data = tmp;
  REQUIRE(data.get<std::vector<uint8_t>>() == tmp);
}

TEST_CASE("array test", "[value]") {
  Array array;
  REQUIRE(array.getType() == ValueType::Array);
  REQUIRE(array.size() == 0);
  array.push_back(makeData(true));
  REQUIRE(array.size() == 1);
  array.erase(0);
  REQUIRE(array.size() == 0);
}

TEST_CASE("map test", "[value]") {
  Map map;
  REQUIRE(map.getType() == ValueType::Map);
  REQUIRE(map.contains("key") == false);
  map.set("key", makeData("hello"));
  REQUIRE(map.contains("key") == true);
  map.erase("key");
  REQUIRE(map.contains("key") == false);
}

TEST_CASE("dataref test", "[value]") {
    std::vector<uint8_t> buf{'h', 'e', 'l', 'l', 'o'};
    auto dr = makeData(buf);
    Elti el(std::move(dr));
    std::string result = el.seriToString();
    el = Elti::parseToElti(result.data(), ParseRef::On).second;
    ValueWrapper vw = el.getRoot();
    REQUIRE(vw.getType() == "dataref");
    REQUIRE(vw.get<std::vector<uint8_t>>() == buf);
}
