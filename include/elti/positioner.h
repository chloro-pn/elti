#pragma once

#include "elti/util.h"
#include "elti/elti_num.h"
#include "elti/elti_type_trait.h"
#include "elti/seri_parse.h"
#include "elti/key.h"
#include <vector>
#include <string>
#include <cassert>

namespace elti {
template<typename Inner>
class Positioner {
public:
  Positioner(Inner& inner, size_t index, ValueType type, bool find = true):
    inner_(inner), index_(index), type_(type), total_size_(0), find_(find) {
    size_t offset = 0;
    if(type != ValueType::Invalid) {
      total_size_ = parseLength(inner_, offset);
      index_ += offset;
    }
  }

  Positioner operator[](const char *attr) {
    assert(type_ == ValueType::Map);
    size_t offset = 0;
    inner_.setIndex(index_);
    uint64_t count = parseLength(inner_, offset);
    for(uint64_t i = 0; i < count; ++i) {
      Key key("");
      key.keyParse(inner_, offset);
      ValueType type = parseValueType(inner_, offset);
      if(key == Key(attr, strlen(attr))) {
        return Positioner(inner_, inner_.curIndex(), type);
      }
      else {
        uint64_t total_size = parseLength(inner_, offset);
        inner_.skip(total_size);
      }
    }
    return Positioner(inner_, 0, ValueType::Invalid, false);
  }

  Positioner operator[](num index) {
    assert(type_ == ValueType::Array);
    size_t offset = 0;
    inner_.setIndex(index_);
    uint64_t count = parseLength(inner_, offset);
    if(index.n_ >= count) {
      return Positioner(inner_, 0, ValueType::Invalid, false);
    }
    for(uint64_t i = 0; i < count; ++i) {
      ValueType type = parseValueType(inner_, offset);
      if(i < index.n_) {
        uint64_t total_size = parseLength(inner_, offset);
        inner_.skip(total_size);
      }
      else {
        assert(i == index.n_);
        return Positioner(inner_, inner_.curIndex(), type);
      }
    }
    return Positioner(inner_, 0, ValueType::Invalid, false);
  }

  uint64_t size() const {
    assert(type_ == ValueType::Array);
    inner_.setIndex(index_);
    size_t offset = 0;
    uint64_t count = parseLength(inner_, offset);
    return count;
  }

  template<typename T>
  T get() const {
    static_assert (detail::NoRefNoPointer<T>::value, "get() method should not return & or * type"
                                                     "(including <T* const> and <const T*> type).\n");
    assert(type_ == ValueType::Data);
    size_t offset = 0;
    inner_.setIndex(index_);
    DataType dt = parseDataType(inner_, offset);
    assert(detail::DataTypeTrait<T>::dt == dt);
    return parse<T>(inner_.curAddr(), static_cast<size_t>(total_size_ - 1));
  }

  bool IsFind() const {
    return find_;
  }

  uint64_t getTotalSize() const {
    return total_size_;
  }

  ValueType getType() const {
    return type_;
  }

private:
  Inner& inner_;
  size_t index_;
  ValueType type_;
  uint64_t total_size_;
  bool find_;
};
}
