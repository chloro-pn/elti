#include <iostream>
#include <vector>
#include "elti.h"
#include "elti/positioner_root.h"
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
#include <ctime>

int main() {
  time_t start,end;
  size_t size = 0;
  start = clock();
  for(int i = 0; i < 10000; ++i) {
    elti::Map* map = elti::makeMap();
    map->set("name", elti::makeData("nanpang"));
    map->set("age", elti::makeData(elti::varintNum(25)));
    map->set("sex", elti::makeData(elti::varintNum(1)));
    map->set("eof", elti::makeData(false));
    elti::Data* data = elti::makeData(elti::varintNum(0));
    map->set("flow_id", data);
    //map->set("content", elti::makeData(std::string(4096, 'a')));
/*
    elti::Array* array = elti::makeArray();
    for(uint32_t i= 0; i < 10; ++i) {
      std::string tmp;
      tmp.resize(4096, 'a');
      array->push_back(elti::makeData(tmp));
    }
    map->set("ids", array);
*/
    elti::Root root(map);
    std::string result;
    root.seri(result);

    size = result.size();
    elti::Root new_root;
    size_t offset = new_root.parse(result.data());
    assert(offset == result.size());
    //new_root["name"].get<std::string>();
    //new_root["content"].get<std::string>();
    //elti::PositionerRoot pst(result.data());
    //pst["name"].get<std::string>();
  }
  end = clock();
  std::cout << " use seconds : " << ((double)(end) - start) / CLOCKS_PER_SEC << std::endl;
  std::cout << " size : " << size << std::endl;
  return 0;
}
