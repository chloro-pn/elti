#pragma once
#include <string>

namespace elti {
class Key {
  friend class Element;
public:
  explicit Key(std::string key);

  Key (const char* ptr, size_t n);

  Key(Key&&) = default;

  Key(const Key&) = default;

  Key& operator=(Key&&) = default;

  Key& operator=(const Key&) = default;

  void keyParse(const char*& ptr, size_t& offset);

  void keySeri(std::string& str);

private:
  std::string key_;
};
}

