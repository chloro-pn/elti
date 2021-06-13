#include "catch.hpp"
#define private public
#define protected public
#include "elti.h"
#include <cstring>

using namespace elti;

TEST_CASE("element test", "[element]") {
  auto ptr = std::unique_ptr<Data>(new Data());
  Element element("key", std::move(ptr));
  REQUIRE(strcmp(element.getKey(), "key") == 0);
  Element e2;
  REQUIRE(e2.getValue() == nullptr);
  REQUIRE(strcmp(e2.getKey(), "") == 0);

  e2 = std::move(element);
  REQUIRE(strcmp(e2.getKey(), "key") == 0);
  REQUIRE(element.getValue() == nullptr);
}
