#include "catch.hpp"
#include "elti/root.h"
#include "elti/factory.h"
#include "elti/value_wrapper.h"

using namespace elti;

TEST_CASE("root test", "[root]") {
  Root root(makeData("hello"));
  REQUIRE(root.get<std::string>() == "hello");

  Array* array = makeArray();
  array->push_back(makeData(true));
  array->push_back(makeData("123"));
  array->push_back(makeData(uint8_t(12)));
  Root r2(array);
  REQUIRE(r2.size() == 3);
  REQUIRE(r2[num(1)].get<std::string>() == "123");
  REQUIRE(r2[num(2)].get<uint8_t>() == 12);

  Map* map = makeMap();
  map->set("name", makeData("nanpang"));
  map->set("age", makeData(uint8_t(25)));
  Array* books = makeArray();
  books->push_back(makeData("b1"));
  books->push_back(makeData("b2"));
  map->set("books", books);
  Root r3(map);
  REQUIRE(r3["books"][num(1)].get<std::string>() == "b2");
}
