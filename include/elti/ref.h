#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace elti {
template<typename T>
const void* getAddr(const T& t);

template<typename T>
size_t getLength(const T& t);

class ref {
public:
  ref(const void* ptr, size_t n) : ptr_(ptr), length_(n) {

  }

  ref(const char* ptr, size_t n) : ptr_(ptr), length_(n) {

  }

  ref(const std::string& str) : ptr_(static_cast<const void*>(str.data())), length_(str.length()) {

  }

  ref(const std::vector<uint8_t>& bin) : ptr_(static_cast<const void*>(&bin.front())), length_(sizeof(uint8_t) * bin.size()) {

  }

  template<typename T>
  ref(const T& t) : ptr_(getAddr(t)), length_(getLength(t)) {

  }

  const void* get_addr() const {
    return ptr_;
  }

  size_t get_length() const {
    return length_;
  }

private:
  const void* ptr_;
  size_t length_;
};
}
