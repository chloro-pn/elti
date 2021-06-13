#include "elti/value_wrapper.h"
#include "elti/util.h"

namespace elti {
  ValueWrapper::ValueWrapper(Value* v, ParseRef ref) : v_(v), ref_(ref) {

  }

  const std::vector<uint8_t>& ValueWrapper::bytesRef() const {
      return getValueAsData(const_cast<Value*>(v_))->bytesRef();
  }

  const char* ValueWrapper::getPtr() const {
      return getValueAsDataRef(const_cast<Value*>(v_))->getPtr();
  }

  size_t ValueWrapper::getLength() const {
      return getValueAsDataRef(const_cast<Value*>(v_))->getLength();
  }

  ValueWrapper ValueWrapper::operator[](const char* attr) const {
    return ValueWrapper(getValueAsMap(v_)->operator[](attr), ref_);
  }

  size_t ValueWrapper::size() const {
    return getValueAsArray(v_)->size();
  }

  ValueWrapper ValueWrapper::operator[](num index) const {
    return ValueWrapper(getValueAsArray(v_)->operator[](index.n_), ref_);
  }
}
