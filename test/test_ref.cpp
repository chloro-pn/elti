#include "elti/ref.h"
#include "elti.h"
#include "catch.hpp"

using namespace elti;

TEST_CASE("ref string test", "[ref]") {
  std::string str(1024, 'a');
  Data* data = makeData(ref(str));
  Root root(data);
  std::string result;
  root.seri(result);
  
  Root new_root;
  new_root.parse(result.data());
  REQUIRE(new_root.get<std::string>() == str);
}

TEST_CASE("ref bin test", "[ref]") {
  std::vector<uint8_t> bin = { 0x01, 0x24, 0xbc };
  Data* data = makeData(ref(bin));
  Root root(data);
  std::string result;
  root.seri(result);

  Root new_root;
  new_root.parse(result.data());
  REQUIRE(new_root.get<std::vector<uint8_t>>() == bin);
}

class test_for_ref {
public:
  std::vector<char> bytes;

};

namespace elti {
  template<>
  const void* getAddr(const test_for_ref& obj) {
    return static_cast<const void*>(obj.bytes.data());
  }

  template<>
  size_t getLength(const test_for_ref& obj) {
    return obj.bytes.size() * sizeof(char);
  }

  template<>
  test_for_ref parse(const std::vector<uint8_t>& container) {
    test_for_ref obj;
    obj.bytes.resize(container.size());
    memcpy(&obj.bytes.front(), container.data(), container.size());
    return obj;
  }
}

TEST_CASE("custom ref", "[ref]") {
  test_for_ref obj;
  obj.bytes = { 'a', 'b', 'c' };
  Data* data = makeData(ref(obj));
  Root root(data);
  std::string result;
  root.seri(result);

  Root new_root;
  new_root.parse(result.data());
  REQUIRE(new_root.get<test_for_ref>().bytes == obj.bytes);
}
