#include "catch.hpp"
#include "elti/key.h"

using namespace elti;

TEST_CASE("key test", "[key]") {
  Key key("key");
  REQUIRE(key == Key("key"));
  std::string k("key");
  REQUIRE(key == Key(k));
}
