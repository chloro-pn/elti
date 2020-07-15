# elti

## elti是什么？
一个基于c++11标准的序列化/反序列化协议与实现，与json，xml等文本协议相比，elti可以传递二进制数据；与protobuf协议相比，elti配置简单，即拿即用。

elti是类似于bson和cbor的序列化协议，相较于bson协议空间利用率不理想（https://blog.csdn.net/m0_38110132/article/details/77716792）
，cbor的c/c++实现又太少（没有找到广泛使用的稳定的轮子），
elti旨在提供一个在**效率，接口易用性，扩展性，空间利用率**等方面有一个均衡稳定的表现，elti主要用来做数据传输而不是展示，即elti**不考虑可读性**。

## 格式
Element := Key : Value

Value := 

       | Map

       | Array

       | Data

Map := { Element }

ARRAY := { Value }

Data := [ byte0, byte1, ... byten ]

// Data : 代表一段二进制序列

## 支持平台
linux, macOS

## 第三方库
elti使用 https://github.com/sorribas/varint.c 作为对整形数据进行变长编码的库。

## Building
just run make. 配置库文件+头文件路径即可使用。

## Test
还没有做全面的测试，计划补充

## 使用
```c++
#include "elti.h"
```
大多数情况下导入elti.h头文件即可

```c++
elti::Map* map = elti::makeMap();
map->set("name", elti::makeData("nanpang"));
map->set("age", elti::makeData(27));
map->set("sex", elti::makeData(1));
map->set("eof", elti::makeData(false));
elti::Data* data = elti::makeData(elti::varintNum(14553));
map->set("flow_id", data);
std::string content(4096, 'a');
map->set("content", elti::makeData(content));
elti::Array* array = elti::makeArray();
for(int i= 0; i < 10; ++i) {
  array->push_back(elti::makeData(i));
}
map->set("ids", array);
```
构建需要序列化的数据，通过elti::makeMap, elti::makeArray和elti::makeData三个API去构造元对象，并通过元对象间的组合形式构建数据的结构。
elti的结构在逻辑上和json相似，同样采取key-value的形式（这里的value可以是Map， Array或者Data），一个Map对象可以存储key-value的集合，
通过set接口一个Array对象可以存储value的集合，通过push_back接口。一个Data对象存储具体的数据，包括
字符串，int8_t,uint8_t,...int64_t,uint64_t,varintNum,二进制数据（std::vector<uint8_t>）bool等类型。

注：
* varintNum类型是变长编码的整形数据，用以压缩空间。
* elti目前不支持浮点数的序列化/反序列化
* 二进制数据通过std::vector<uint8_t>或者std::string传递。
* 所有通过set，push_back接口放入的对象由elti负责释放，不需要使用者释放。

```c++
elti::Root root(map);
std::string result;
root.seri(result);
```
通过使用Map类型对象，Array类型对象或者Data类型对象构造elti::Root对象（同样由elti负责释放此根对象），通过seri接口完成序列化。

```c++
elti::Root new_root;
size_t offset = new_root.parse(result.data());
assert(offset == result.size());
```
通过默认构造函数构造elti::Root对象，通过parse接口完成反序列化，该接口返回的数据为序列化数据的总长度，在没有发生错误的情况下应该与传入的序列化数据长度相等。

```c++
auto arr = new_root["ids"];
for(int i = 0; i < arr.size(); ++i) {
  std::cout << "ids index : " << i << " id : " << arr[elti::num(i)].get<int>() << std::endl;
}
std::cout << "flow id : " << new_root["flow_id"].get<elti::varintNum>().getNum() << std::endl;
std::cout << "name : " << new_root["name"].get<std::string>() << std::endl;
std::cout << "content size : " << new_root["content"].get<std::string>().size() << std::endl;
```

数据的访问，Root类型重载了operator[]，访问Map对象通过operator[](const char*)接口，访问Array对象通过operator[](elti::num)接口和size()接口，
访问Data对象通过.get<TYPE>()接口。
注：
* Root假设用户按照正确的类型访问对应的对象，通过assert进行判断，如果通过接口访问了不对应的类型，debug模式下会报错，release模式下结果未定义。

## 变长编码

elti在存储元信息时使用变长编码以节省空间，用户也可以通过传递给Data类型varintNum类型对象以使用变长编码传递整形数据（目前变长编码仅支持无符号类型）：
```c++
elti::Data* data = elti::makeData(elti::varintNum(14553)); //编码
std::cout << "flow id : " << new_root["flow_id"].get<elti::varintNum>().getNum() << std::endl; //解码
```

## 自定义序列化/反序列化接口

elti仅提供基本类型的序列化机制，你也可以通过实例化以下两个模板参数来定制特定类型的序列化，反序列化接口：
```c++
template<typename T>
void seri(const T& obj, std::vector<uint8_t>& container);

template<typename T>
T parse(const std::vector<uint8_t>& container);
```

例子：
```c++
class test {
public:
  test(int a, std::string n): age(a), name(n) {

  }
  //just for test.
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
  //调用自定义的seri接口
  obj->set("obj", elti::makeData(test(25, "nanpang")));
  elti::Root root(obj);
  std::string result;
  root.seri(result);
  
  elti::Root new_root;
  size_t offset = new_root.parse(result.data());
  assert(offset == result.length());
  //调用自定义的parse接口
  test t = new_root["obj"].get<test>();
  std::cout << "age : " << t.age << " name : " << t.name << std::endl;
  return 0;
}
```
