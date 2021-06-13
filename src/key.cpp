#include "elti/key.h"
#include "elti/util.h"
#include <cstring>

namespace elti {
Key::Key(const std::string& key) : key_(key) {

}

Key::Key(const char* ptr, size_t n) : key_(ptr, n) {

}

void Key::keyParse(const char *&begin, size_t& offset) {
  uint64_t length = parseLength(begin, offset);
  key_.resize(length);
  memcpy(&(*key_.begin()), begin, length);
  begin += length;
  offset += length;
}

void Key::keySeri(std::string &result) const {
  uint32_t key_length = key_.size();
  seriLength(key_length, result);
  result.append(key_);
}
}
