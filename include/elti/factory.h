#pragma once
#include "elti/value.h"
#include <memory>

namespace elti {
template<typename... Args>
std::unique_ptr<Data> makeData(Args&&... args) {
  return std::unique_ptr<Data>(new Data(std::forward<Args>(args)...));
}

std::unique_ptr<Map> makeMap();

std::unique_ptr<Array> makeArray();
}
