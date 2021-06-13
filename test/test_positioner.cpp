#include "catch.hpp"
#define private public
#define protected public
#include "elti.h"

using namespace elti;

TEST_CASE("positioner test", "[positioner]") {
  auto array = makeArray();
  array->push_back(makeData("string"));
  array->push_back(makeData(false));
  array->push_back(makeData(varintNum(23)));
  auto map = makeMap();
  map->set("test", std::move(array));
  map->set("name", makeData("bob"));
  Elti root(std::move(map));
  std::string result = root.seriToString();
  Positioner pst = elti::Elti::parseToPositioner(result.data());
  REQUIRE(pst.getType() == ValueType::Map);
  REQUIRE(pst["name"].getType() == ValueType::Data);
  REQUIRE(pst["name"].get<std::string>() == "bob");
  REQUIRE(pst["nothing"].IsFind() == false);
  REQUIRE(pst["test"][num(1)].get<bool>() == false);
  REQUIRE(pst["test"][num(3)].IsFind() == false);
  REQUIRE(pst["t2"].IsFind() == false);
}
