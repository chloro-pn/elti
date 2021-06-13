#include <iostream>
#include <vector>
#include "elti.h"

int main() {
  auto map = elti::makeMap();
  map->set("name", elti::makeData("bob"));
  map->set("age", elti::makeData(elti::varintNum(25)));
  map->set("sex", elti::makeData(elti::varintNum(1)));
  map->set("eof", elti::makeData(false));
  auto data = elti::makeData(elti::varintNum(0));
  map->set("flow_id", std::move(data));
  auto arr = elti::makeArray();
  arr->push_back(elti::makeData(1));
  arr->push_back(elti::makeData(2));
  map->set("arrs", std::move(arr));
  elti::Elti el(std::move(map));
  std::string result;
  el.seriTo<std::string>(result);

  auto rs = elti::Elti::parseToElti(elti::InnerWrapper(result), elti::ParseRef::Off);
  assert(rs.first == result.size());
  elti::ValueWrapper nroot = rs.second.getRoot();
  std::cout << "root type should be map. root type : " << nroot.getType() << std::endl;
  std::cout << "root[\"name\"] should be \"bob\". root[name] = " <<  nroot["name"].get<std::string>() << std::endl;
  std::cout << "root[\"arrs\"].size() should be 2. nroot[arrs].size() = " << nroot["arrs"].size() << std::endl;
  std::cout << nroot["age"].get<elti::varintNum>().getNum() << std::endl;
  std::cout << nroot.toJson(elti::BytesEncode::HexStyle) << std::endl;

  DataType dt = elti::detail::DataTypeTrait<double>::dt;
  if (dt == ELTI_CUSTOM) {
    std::cout << "data type trait success!" << std::endl;
  }
  return 0;
}
