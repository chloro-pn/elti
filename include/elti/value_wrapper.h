#pragma once
#include "value.h"
#include "util.h"

namespace elti {
  struct num {
    size_t n_;
    explicit num(size_t n) : n_(n) {

    }
  };

  struct ValueWrapper {
    Value* v_;
    explicit ValueWrapper(Value* v);

    template<typename T>
    T get() {
      return getValueAsData(v_)->get<T>();
    }

    template<typename T>
    void setData(T&& arg) {
      getValueAsData(v_)->setData(std::forward<T>(arg));
    }

    template<typename T>
    ValueWrapper& operator=(T&& other) {
      setData(std::forward<T>(other));
      return *this;
    }

    size_t useBytes() const;

    ValueWrapper operator[](const char* attr);

    size_t size() const;

    ValueWrapper operator[](num index);
  };
}
