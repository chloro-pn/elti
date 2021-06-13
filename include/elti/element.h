#pragma once

#include <cstring>
#include <vector>
#include <memory>
#include "elti/key.h"
#include "elti/elti_enum.h"

namespace elti {
class Value;
class Element {
  friend class Elti;
  friend class Map;
  friend class Array;
  friend class Data;

private:
  Key key_;
  std::unique_ptr<Value> v_;

  Value* getValue();
  const Value* getValue() const;

public:
  Element();
  Element(const char* ptr, std::unique_ptr<Value>&& v);
  Element(const Element&) = delete;
  Element& operator=(const Element&) = delete;
  Element(Element&& other) noexcept;
  Element& operator=(Element&& other) noexcept;

  const char* getKey() const {
    return key_.key_.c_str();
  }

  void parse(const char*& begin, size_t& offset, ParseRef ref);
  void seri(std::string& result) const;
  ~Element();
};
}

