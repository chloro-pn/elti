#include "catch.hpp"
#define private public
#define protected public
#include "elti.h"

using namespace elti;

TEST_CASE("parseref test", "[parse]") {
  auto map = makeMap();
  map->set("name", makeData("bob"));
  map->set("age", makeData(static_cast<uint32_t>(25)));
  map->set("school", makeData("hust"));
  auto arr = makeArray();
  arr->push_back(makeData("chinese"));
  arr->push_back(makeData("english"));
  map->set("language", std::move(arr));
  Elti el(std::move(map));
  std::string result = el.seriToString();

  auto rs = Elti::parseToElti(result.data(), ParseRef::On);
  REQUIRE(rs.first == result.size());
  ValueWrapper root = rs.second.getRoot();
  REQUIRE(root.getType() == "map");
  REQUIRE(root["name"].get<std::string>() == "bob");
  REQUIRE(root["age"].get<uint32_t>() == static_cast<uint32_t>(25));
  REQUIRE(root["language"].getType() == "array");
  REQUIRE(root["name"].getType() == "dataref");
  REQUIRE(root["age"].getType() == "dataref");
  REQUIRE(root["school"].getType() == "dataref");
  REQUIRE(root["language"][num(0)].getType() == "dataref");
}
