#include "catch.hpp"
#define private public
#define protected public
#include "elti.h"
#include <cstring>

using namespace elti;

TEST_CASE("data constructor test", "[value]") {
  std::vector<uint8_t> container;
  char bytes[10] = "hello !";
  Data d1(bytes);
  REQUIRE(d1.data_type_ == ELTI_STRING);
  seri(d1.data_, container);

  // const char [n].
  Data d2("hello !");
  REQUIRE(d2.data_type_ == ELTI_STRING);
  seri(d2.data_, container);

  const char* ptr = bytes;
  Data d3(ptr);
  REQUIRE(d3.data_type_ == ELTI_STRING);
  seri(d3.data_, container);

  char* ptr2 = bytes;
  Data d4(ptr2);
  REQUIRE(d4.data_type_ == ELTI_STRING);
  seri(d4.data_, container);
}
