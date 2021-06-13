#include "catch.hpp"
#define private public
#define protected public
#include "elti/elti.h"
#include "elti/factory.h"
#include "elti/value_wrapper.h"
#include "elti/inner_wrapper.h"

using namespace elti;

TEST_CASE("root test", "[root]") {
  Elti el(makeData("hello"));
  ValueWrapper r = el.getRoot();
  REQUIRE(r.getType() == "data");
  REQUIRE(r.get<std::string>() == "hello");

  auto array = makeArray();
  array->push_back(makeData(true));
  array->push_back(makeData("123"));
  array->push_back(makeData(uint8_t(12)));
  Elti el2(std::move(array));
  ValueWrapper r2 = el2.getRoot();
  REQUIRE(r2.size() == 3);
  REQUIRE(r2[num(1)].get<std::string>() == "123");
  REQUIRE(r2[num(2)].get<uint8_t>() == 12);

  auto map = makeMap();
  map->set("name", makeData("bob"));
  map->set("age", makeData(uint8_t(25)));
  auto books = makeArray();
  books->push_back(makeData("b1"));
  books->push_back(makeData("b2"));
  map->set("books", std::move(books));
  Elti el3(std::move(map));
  ValueWrapper r3 = el3.getRoot();
  REQUIRE(r3["books"][num(1)].get<std::string>() == "b2");
}

TEST_CASE("root reset test", "[root]") {
  auto array = makeArray();
  array->push_back(makeData(true));
  array->push_back(makeData("bob"));
  auto map = makeMap();
  map->set("student", std::move(array));
  Elti root(std::move(map));
  //move to the other obj.
  Elti r2(std::move(root));
  std::unique_ptr<Value> vp = r2.reset();
  getValueAsMap(vp.get())->set("age", makeData(uint8_t(23)));
  Elti r3(std::move(vp));
  std::string result;
  r3.seriTo<std::string>(result);

  Elti r4 = elti::Elti::parseToElti(InnerWrapper(result)).second;
  ValueWrapper root4 = r4.getRoot();
  REQUIRE(root4["age"].get<uint8_t>() == 23);
  REQUIRE(root4["student"][num(1)].get<std::string>() == "bob");
  REQUIRE(root4["student"][num(0)].get<bool>() == true);
  REQUIRE(root4["student"].size() == 2);
}
