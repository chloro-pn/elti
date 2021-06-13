#include "elti.h"
#include <cstdint>
#include <string>
#include <cassert>
#include <iostream>

struct T1 {
  uint8_t age;
  std::string name;

  std::unique_ptr<elti::Value> seriToValue() const {
    auto v = elti::makeMap();
    v->set("type", elti::makeData("T1"));
    v->set("age", elti::makeData(age));
    v->set("name", elti::makeData(name));
    return v;
  }

  static T1 parseFromValue(const elti::ValueWrapper& v) {
    T1 t;
    assert(v.getType() == "map" && v["type"].get<std::string>() == "T1");
    t.age = v["age"].get<uint8_t>();
    t.name = v["name"].get<std::string>();
    return t;
  }
};

int main() {
  T1 t;
  t.age = 18;
  t.name = "bob";
  elti::Elti el(t.seriToValue());
  std::string result;
  el.seriTo<std::string>(result);

  auto rs = elti::Elti::parseToElti(elti::InnerWrapper(result));
  assert(rs.first == result.size());
  T1 t2 = T1::parseFromValue(rs.second.getRoot());
  std::cout << "t2.age should be 18. " << (int)t2.age << std::endl;
  std::cout << "t2.name should be bob. " << t2.name << std::endl;
  return 0;
}
