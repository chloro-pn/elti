#pragma once
#include <string>

namespace elti {
class Key {
  friend class Element;

public:
  explicit Key(const std::string& key);
  Key (const char* ptr, size_t n);
  Key(Key&&) = default;
  Key(const Key&) = default;
  Key& operator=(Key&&) = default;
  Key& operator=(const Key&) = default;

  bool operator==(const Key& other) const {
    return key_ == other.key_;
  }

  void keyParse(const char*& ptr, size_t& offset);
  void keySeri(std::string& str) const;

private:
  std::string key_;
};
}

