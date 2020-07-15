#include <iostream>
#include <string>
#include <vector>
#include "elti.h"

class test {
public:
  test(int a, std::string n): age(a), name(n) {

  }

  int age;
  std::string name;
};

namespace elti {
template<>
void seri(const test& obj, std::vector<uint8_t>& container) {
  container.resize(sizeof(obj.age) + obj.name.size());
  memcpy(&container.front(), &obj.age, sizeof(obj.age));
  memcpy(&container.front() + sizeof(obj.age), obj.name.data(), obj.name.size());
}

template<>
test parse(const std::vector<uint8_t>& container) {
  int age;
  std::string name;
  memcpy(&age, &container.front(), sizeof(age));
  name.append((char*)(&container.front() + sizeof(age)), container.size() - sizeof(age));
  return test(age, name);
}
}

int main() {
  elti::Map* obj = elti::makeMap();
  obj->set("obj", elti::makeData(test(25, "nanpang")));
  elti::Root root(obj);
  std::string result;
  root.seri(result);

  elti::Root new_root;
  size_t offset = new_root.parse(result.data());
  assert(offset == result.length());

  test t = new_root["obj"].get<test>();
  std::cout << "age : " << t.age << " name : " << t.name << std::endl;
  return 0;
}
