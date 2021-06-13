#include <iostream>
#include <string>
#include <vector>
#include "elti.h"

class test {
public:
  test(int8_t f, std::string a, std::string s):floor_(f),
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
test parse(const char* ptr, size_t n) {
  int8_t floor;
  memcpy(&floor, ptr, sizeof(floor));
  return test(floor, "unknow", "unknow");
}
}

int main() {
  auto obj = elti::makeArray();
  obj->push_back(elti::makeData("book1")); // add.
  obj->push_back(elti::makeData("book2"));
  obj->push_back(elti::makeData("error"));
  obj->erase(2); // delete.
  auto message = elti::makeMap();
  auto name = elti::makeData("bob");
  (*name) = "chalice"; // change.
  message->set("name", std::move(name));
  message->set("age", elti::makeData(elti::varintNum(25)));
  message->set("position", elti::makeData(test(39, "yfnx", "029")));
  message->set("books", std::move(obj));

  elti::Elti root(std::move(message));
  std::string result = root.seriToString();

  elti::Positioner pst = elti::Elti::parseToPositioner(result.data());
  test t = pst["position"].get<test>();
  std::cout << (int)t.floor_ << std::endl;
  return 0;
}
