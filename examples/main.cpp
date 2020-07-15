#include <iostream>
#include <vector>
#include "elti.h"
// Element := Key : Value
//
// Value := Map
//          Array
//          Data
// Map := { Element }
// ARRAY = { Value }
// Data = [ byte0, byte1, ... byten ]

// Data : an array of byte. [byte0, byte1, ... byten]
// ARRAY : an array of Value

int main() {
  elti::Map* map = elti::makeMap();
  map->set("name", elti::makeData("nanpang"));
  map->set("age", elti::makeData(elti::varintNum(27)));
  map->set("sex", elti::makeData(elti::varintNum(1)));
  map->set("eof", elti::makeData(false));
  elti::Data* data = elti::makeData(elti::varintNum(14553));
  map->set("flow_id", data);
  std::string content(4096, 'a');
  map->set("content", elti::makeData(content));
  elti::Array* array = elti::makeArray();
  for(int32_t i= 0; i < 10; ++i) {
    array->push_back(elti::makeData(i));
  }
  map->set("ids", array);
  elti::Root root(map);
  std::string result;
  root.seri(result);

  elti::Root new_root;
  size_t offset = new_root.parse(result.data());
  assert(offset == result.size());
  auto arr = new_root["ids"];
  for(int i = 0; i < arr.size(); ++i) {
    std::cout << "ids index : " << i << " id : " << arr[elti::num(i)].get<int32_t>() << std::endl;
  }
  std::cout << "flow id : " << new_root["flow_id"].get<elti::varintNum>().getNum() << std::endl;
  std::cout << "name : " << new_root["name"].get<std::string>() << std::endl;
  std::cout << "content size : " << new_root["content"].get<std::string>().size() << std::endl;
  return 0;
}
