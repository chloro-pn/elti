#pragma once

#include <cstdint>

namespace elti {
class varintNum {
  uint64_t num_;

public:
  explicit varintNum(uint64_t num) : num_(num) {

  }

  uint64_t getNum() const {
    return num_;
  }
};
}
