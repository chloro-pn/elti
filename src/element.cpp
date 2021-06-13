#include "elti/value.h"
#include "elti/util.h"
#include <utility>

namespace elti {
Element::Element():key_(""), v_(nullptr) {

}

Element::Element(const char* ptr, std::unique_ptr<Value>&& v):key_(ptr), v_(std::move(v)) {

}

Element::Element(Element&& other) noexcept :key_(::std::move(other.key_)), v_(std::move(other.v_)) {
  other.v_ = nullptr;
}

Element& Element::operator=(Element &&other) noexcept {
  key_ = std::move(other.key_);
  v_ = std::move(other.v_);
  other.v_ = nullptr;
  return *this;
}

Value* Element::getValue() {
  return v_.get();
}

const Value* Element::getValue() const {
    return v_.get();
}

Element::~Element() {

}
}
