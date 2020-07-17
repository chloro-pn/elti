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
/*
  elti::Root new_root;
  size_t offset = new_root.parse(result.data());
  assert(offset == result.length());

  test t = new_root["obj"].get<test>();
*/
  elti::PositionerRoot pst(result.data());
  //test t = pst[elti::num(0)].get<test>();
  std::string book = pst["books"][elti::num(1)].get<std::string>();
  //std::cout << "age : " << t.age << " name : " << t.name << std::endl;
  std::cout << "book : " << book << std::endl;
  return 0;
}
