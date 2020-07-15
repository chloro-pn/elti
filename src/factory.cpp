#include "elti/factory.h"

namespace elti {
Map* makeMap() {
  return new Map();
}

Array* makeArray() {
  return new Array();
}
}
