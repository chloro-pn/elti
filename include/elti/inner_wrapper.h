#pragma once

#include <cstddef>
#include <string>
#include <cassert>

namespace elti {
class InnerWrapper {
public:
  InnerWrapper(const std::string& str) : str_(str), current_index_(0) {

  }

  const char* curAddr() const {
    assert(current_index_ < str_.size());
    return &str_[current_index_];
  }

  void skip(size_t k) const {
    current_index_ += k;
    assert(current_index_ <= str_.size());
  }

  void setIndex(size_t index) {
    assert(index <= str_.size());
    current_index_ = index;
  }

  size_t curIndex() const {
    return current_index_;
  }

private:
  const std::string& str_;
  mutable size_t current_index_;
};
}
