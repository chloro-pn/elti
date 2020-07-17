#include "elti/value_wrapper.h"
#include "elti/util.h"

namespace elti {
  ValueWrapper::ValueWrapper(Value* v) : v_(v) {

  }

  size_t ValueWrapper::useBytes() const {
    return getValueAsData(v_)->useBytes();
  }

  ValueWrapper ValueWrapper::operator[](const char* attr) {
    return ValueWrapper(getValueAsMap(v_)->operator[](attr));
  }

  size_t ValueWrapper::size() const {
    return getValueAsArray(v_)->size();
  }

  ValueWrapper ValueWrapper::operator[](num index) {
    return ValueWrapper(getValueAsArray(v_)->operator[](index.n_));
  }
}
