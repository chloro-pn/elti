#include "catch.hpp"
#include "elti/elti_type_trait.h"
#include "elti/elti_enum.h"
#include <string>

using namespace elti::detail;

TEST_CASE("type trait test", "[type_trait]") {
  REQUIRE(NoRefNoPointer<int>::value);
  REQUIRE(NoRefNoPointer<const int>::value);
  REQUIRE(NoRefNoPointer<int&>::value == false);
  REQUIRE(NoRefNoPointer<const int&>::value == false);
  REQUIRE(NoRefNoPointer<int&&>::value == false);
  REQUIRE(NoRefNoPointer<const int&&>::value == false);
  REQUIRE(NoRefNoPointer<int*>::value == false);
  REQUIRE(NoRefNoPointer<const int*>::value == false);
  REQUIRE(NoRefNoPointer<int* const>::value == false);
  REQUIRE(NoRefNoPointer<const int* const>::value == false);

  REQUIRE(DataTypeTrait<std::string>::dt == ELTI_STRING);
}
