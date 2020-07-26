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

TEST_CASE("root reset test", "[root]") {
  Array* array = makeArray();
  array->push_back(makeData(true));
  array->push_back(makeData("nanpang"));
  Map* map = makeMap();
  map->set("student", array);
  Root root(map);
  //move to the other obj.
  Root r2(std::move(root));
  Value* vp = r2.reset();
  getValueAsMap(vp)->set("age", makeData(uint8_t(23)));
  Root r3(vp);
  std::string result;
  r3.seri(result);

  Root r4;
  r4.parse(result.data());
  REQUIRE(r4["age"].get<uint8_t>() == 23);
  REQUIRE(r4["student"][num(1)].get<std::string>() == "nanpang");
  REQUIRE(r4["student"][num(0)].get<bool>() == true);
  REQUIRE(r4["student"].size() == 2);
}