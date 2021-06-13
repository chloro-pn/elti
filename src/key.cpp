#include "elti/key.h"
#include "elti/util.h"
#include <cstring>

namespace elti {
Key::Key(const std::string& key) : key_(key) {

}

Key::Key(const char* ptr, size_t n) : key_(ptr, n) {

}
}
