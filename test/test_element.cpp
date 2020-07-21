#include "catch.hpp"
#include "elti/element.h"
#include "elti/value.h"
#include <cstring>

using namespace elti;

TEST_CASE("element test", "[element]") {
  Value* ptr = new Data();
  Element element("key", ptr);
  REQUIRE(element.getValue() == ptr);
  REQUIRE(strcmp(element.getKey(), "key") == 0);
  Element e2;
  REQUIRE(e2.getValue() == nullptr);
  REQUIRE(strcmp(e2.getKey(), "") == 0);

  e2 = std::move(element);
  REQUIRE(e2.getValue() == ptr);
  REQUIRE(strcmp(e2.getKey(), "key") == 0);
  REQUIRE(element.getValue() == nullptr);
}
