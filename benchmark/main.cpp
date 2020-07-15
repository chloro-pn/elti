#include <iostream>
#include <vector>
#include "elti.h"
#include "jemalloc/jemalloc.h"
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

void* operator new(size_t size) {
    void* m = malloc(size);
    return m;
}

void operator delete(void* m) {
    free(m);
}

int main() {
  time_t start, end;
  start = clock();
  for(int i = 0; i < 100000; ++i) {
    elti::Map* map = elti::makeMap();
    map->set("name", elti::makeData("nanpang"));
    map->set("age", elti::makeData(27));
    map->set("sex", elti::makeData(1));
    map->set("eof", elti::makeData(false));
    map->set("flow_id", elti::makeData(0));
    std::string content(4096, 'a');
    map->set("conent", elti::makeData(content));
    elti::Array* array = elti::makeArray();
    for(int i= 0; i < 10; ++i) {
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
      arr[elti::num(i)].get<int>();
    }

    //new_root["name"].get<std::string>();
    //new_root["content"].get<std::string>();
  }
  end = clock();
  std::cout << "use time : " << ((double)(end) - start) / CLOCKS_PER_SEC << std::endl;
  return 0;
}
