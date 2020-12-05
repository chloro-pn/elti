#include <iostream>
#include <string>
#include <vector>
#include "elti.h"
#include "elti/positioner_root.h"

class test {
public:
  test(int8_t f, std::string a, std::string s):
                                              floor_(f),
                                              apartment_(a),
                                              station_number_(s) {

  }

  int8_t floor_;
  std::string apartment_;
  std::string station_number_;
};

namespace elti {
template<>
void seri(const test& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj.floor_));
  memcpy(&container.front(), &obj.floor_, sizeof(obj.floor_));
}

template<>
test parse(const std::vector<uint8_t>& container) {
  int8_t floor;
  memcpy(&floor, &container.front(), sizeof(floor));
  return test(floor, "unknow", "unknow");
}

template<>
test parse(const char* ptr, size_t n) {
  int8_t floor;
  memcpy(&floor, ptr, sizeof(floor));
  return test(floor, "unknow", "unknow");
}
}

int main() {
  elti::Array* obj = elti::makeArray();
  obj->push_back(elti::makeData("book1")); // add.
  obj->push_back(elti::makeData("book2"));
  obj->push_back(elti::makeData("error"));
  obj->erase(2); // delete.
  elti::Map* message = elti::makeMap();
  elti::Data* name = elti::makeData("pangnan");
  (*name) = "nanpang"; // change.
  message->set("name", name);
  message->set("age", elti::makeData(elti::varintNum(25)));
  message->set("position", elti::makeData(test(39, "yfnx", "029")));
  message->set("books", obj);

  elti::Root root(message);
  std::string result;
  root.seri(result);

  elti::PositionerRoot pst(result.data());
  test t = pst["position"].get<test>();
  std::cout << (int)t.floor_ << std::endl;
  return 0;
}
