#include <iostream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include <ctime>
using nlohmann::json;

using namespace std;

int main() {
    time_t start, end;
    size_t size = 0;
    start = clock();
    for(int i = 0; i < 10000; ++i) {
	json j;
	j["name"] = "pangnan";
	j["age"] = 26;
	j["sex"] = 1;
	j["eof"] = false;
	j["flow_id"] = 0;
	std::string content(4096, 'a');
	j["content"] = content;
/*
	for(int i = 0; i < 10; ++i) {
	  std::string tmp(4096, 'a');
	  j["ids"].push_back(tmp);
	}
*/
	std::string result = j.dump();
	size = result.size();

    json j2 = json::parse(result);

    //for(json::const_iterator it = j2["ids"].begin(); it != j2["ids"].end(); ++it) {
    //    *it;
    //}
    }
    end = clock();
    std::cout << "use time : " << ((double)(end) - start) / CLOCKS_PER_SEC << std::endl;
    std::cout << "size : " << size << std::endl;
    return 0;
}
