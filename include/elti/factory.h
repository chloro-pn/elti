#pragma once
#include "value.h"

namespace elti {
template<typename... Args>
Data* makeData(Args&&... args) {
  return new Data(std::forward<Args>(args)...);
}

Map* makeMap();

Array* makeArray();
}

