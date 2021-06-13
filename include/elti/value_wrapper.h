#pragma once

#include "elti/value.h"
#include "elti/elti_enum.h"
#include "elti/elti_num.h"

namespace elti {
class ValueWrapper {
private:
  Value* v_;
  ParseRef ref_;

public:
  ValueWrapper(Value* v, ParseRef ref);

  std::string getType() const {
    ValueType vt =  v_->getType();
    switch(vt) {
      case ValueType::Map:
        return "map";
      case ValueType::Array:
        return "array";
      case ValueType::Data:
        return "data";
      case ValueType::DataRef:
        return "dataref";
      default:
        return "invalid";
    }
  }

  std::string toJson(const BytesEncode& be) const {
    return v_->toJson(be);
  }
  // data
  template<typename T>
  T get() const {
    if(ref_ == ParseRef::Off)
      return getValueAsData(v_)->get<T>();
    else
      return getValueAsDataRef(v_)->get<T>();
  }

  const std::vector<uint8_t>& bytesRef() const;
  //dataref
  const char* getPtr() const;
  size_t getLength() const;
  //map
  ValueWrapper operator[](const char* attr) const;
  //array
  size_t size() const;
  ValueWrapper operator[](num index) const;
};
}
