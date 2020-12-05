#include "catch.hpp"
#include "elti/positioner_root.h"
#include "elti.h"

using namespace elti;

TEST_CASE("positioner test", "[positioner]") {
  Array* array = makeArray();
  array->push_back(makeData("string"));
  array->push_back(makeData(false));
  array->push_back(makeData(varintNum(23)));
  Map* map = makeMap();
  map->set("test", array);
  map->set("name", makeData("nanpang"));
  Root root(map);
  std::string result;
  root.seri(result);
  PositionerRoot pst(result.data());
  REQUIRE(pst.getType() == ValueType::Map);
  REQUIRE(pst["name"].getType() == ValueType::Data);
  REQUIRE(pst["name"].get<std::string>() == "nanpang");
  REQUIRE(pst["nothing"].IsFind() == false);
  REQUIRE(pst["test"][num(1)].get<bool>() == false);
  REQUIRE(pst["test"][num(3)].IsFind() == false);
}
