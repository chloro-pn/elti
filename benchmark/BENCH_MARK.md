## 测试环境
测试环境1：
macOS 10.14.6 
Intel Core i5 1.8GHz
Clang 1001.0.46.4

测试环境2：
Tlinux x86_64
AMD 48Core
Gcc 4.8.2

## 参与测试库
elti，protobuf， rapidjson， hlohmann/json。

## 测试内容

------------------------第一种类型数据

name : nanpang

age : 25

sex : 1

eof : false

flow_id : 0

------------------------第二种类型数据，在一的基础上加content

content : string(4096, ‘a’);

------------------------第三种类型数据，在二的基础上加ids

ids : array(string(4096, ‘a’)) // array.size() == 10.

分别测试以上三种类型的消息，每次测试进行1w次消息序列化，反序列化操作，统计消耗时间和序列化后的数据大小。

测试结果见README.md

注：本次测试并不是严格意义上的性能基准测试，因为rapidjson和nlohmann/json需要按照json格式序列化，elti和protobuf等自定义协议的序列化库并不需要为此付出代价，
且nlohmann并不是以性能为核心（文档中自述），此测试结果仅为了说明elti对比其他序列化/反序列化库时的表现，这种结果不仅与实现有关，也与支持协议，能否自描述，
测试数据的特定等相关，换言之这是一份不同序列化库的综合对比，只是性能以量化的方式展示在图中而已。
