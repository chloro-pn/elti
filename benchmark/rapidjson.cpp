#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <ctime>

using namespace rapidjson;

int main() {
  time_t start, end;
  size_t size = 0;
  start = clock();
  for(int i = 0; i < 10000; ++i) {
      Document document;
      document.SetObject();
      Value name("nanpang");
      Value age(27);
      Value sex(1);
      Value eof(false);
      Value flow_id(0);

      std::string str(4096, 'a');

      Value content;
      content.SetString(str.data(), str.length(), document.GetAllocator());

      document.AddMember("age", age, document.GetAllocator());
      document.AddMember("sex", sex, document.GetAllocator());
      document.AddMember("eof", eof, document.GetAllocator());
      document.AddMember("flow_id", flow_id, document.GetAllocator());
      const char* ptr = "name";
      document.AddMember(StringRef(ptr), name, document.GetAllocator());
      document.AddMember("content", content, document.GetAllocator());
/*
      Value ids(kArrayType);
      for(int i = 0; i < 10; ++i) {
	  std::string tmp(4096, 'a');
	  Value v;
	  v.SetString(tmp.data(), tmp.length(), document.GetAllocator());
	  ids.PushBack(v, document.GetAllocator());
      }
      document.AddMember("ids", ids, document.GetAllocator());
*/
      StringBuffer buffer;
      Writer<StringBuffer> writer(buffer);
      document.Accept(writer);

      const char* output = buffer.GetString();
      size = buffer.GetSize();
      StringStream s(output);
      Document d;
      d.ParseStream(s);
/*
      const Value& a = d["ids"];
      for(SizeType i = 0; i < a.Size(); ++i) {
	  a[i].GetString();
      }
*/
  }
  end = clock();
  std::cout << "use time : " << ((double)(end) - start) / CLOCKS_PER_SEC << std::endl;
  std::cout << "size : " << size << std::endl;
  return 0;
}
