#pragma once

#include "key.h"
#include <cstring>
#include <vector>

namespace elti {
class Value;
class Element {
  friend class Map;
  friend class Array;
  friend class Data;
private:
    Key key_;
    Value* v_;

public:
    Element();
    Element(const char* ptr, Value* v);
    Element(Element&& other) noexcept;
    Element& operator=(Element&& other) noexcept;

    const char* getKey() const {
      return key_.key_.c_str();
    }

    Value* getValue();

    void parse(const char*& begin, size_t& offset);

    void seri(std::string& result);

    ~Element();
};
}

