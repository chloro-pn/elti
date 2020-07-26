#include <iostream>
#include "message.pb.h"
#include <string>
#include <ctime>

int main()
{
  time_t start, end;
  start = clock();
  size_t size = 0;
  for(int i = 0; i < 10000; ++i) {
  test tt;
  tt.set_name("nanpang");
  tt.set_age(25);
  tt.set_sex(1);
  tt.set_eof(false);
  tt.set_content(std::string(4096, 'a'));
  tt.set_flow_id(0);
  /*
  for(int i = 0; i < 10; ++i) {
    tt.add_ids(std::string(4096, 'a'));
  }
  */
  std::string str;
  tt.SerializeToString(&str);
  size = str.size();
  test t2;
  t2.ParseFromString(str);
  }
  end = clock();
  std::cout << "use time : " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;
  std::cout << "size : " << size << std::endl;
  return 0;
}
