#include "elti/factory.h"
#include <memory>

namespace elti {
std::unique_ptr<Map> makeMap() {
  return std::unique_ptr<Map>(new Map());
}

std::unique_ptr<Array> makeArray() {
  return std::unique_ptr<Array>(new Array());
}
}
