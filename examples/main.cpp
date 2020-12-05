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
#include <iostream>

using namespace elti;

int main() {
  Map* map = makeMap();
  map->set("name", makeData("nanpang"));
  map->set("age", makeData(varintNum(27)));
  map->set("sex", makeData(varintNum(1)));
  map->set("eof", makeData(false));
  Data* data = makeData(varintNum(14553));
  map->set("flow_id", data);
  std::string content(4096, 'a');
  map->set("content", makeData(content));
  Array* array = makeArray();
  for(int32_t i= 0; i < 10; ++i) {
    array->push_back(makeData(i));
  }
  map->set("ids", array);
  Root root(map);
  std::string result;
  root.seri(result);

  elti::Root new_root;
  size_t offset = new_root.parse(result.data());
  assert(offset == result.size());
  auto arr = new_root["ids"];
  for(int i = 0; i < arr.size(); ++i) {
    std::cout << "ids index : " << i << " id : " << arr[num(i)].get<int32_t>() << std::endl;
  }
  std::cout << "flow id : " << new_root["flow_id"].get<varintNum>().getNum() << std::endl;
  std::cout << "name : " << new_root["name"].get<std::string>() << std::endl;
  std::cout << "content size : " << new_root["content"].get<std::string>().size() << std::endl;
  return 0;
}
