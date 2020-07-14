#include "elti/element.h"
#include "elti/value.h"

namespace elti {
Element::Element():key_(""), v_(nullptr) {

}

Element::Element(const char* ptr, Value* v):key_(ptr), v_(v) {

}

Element::Element(Element&& other) noexcept :key_(std::move(other.key_)), v_(other.v_) {
    other.v_ = nullptr;
}

Element& Element::operator=(Element &&other) noexcept {
    key_ = std::move(other.key_);
    v_ = other.v_;
    other.v_ = nullptr;
    return *this;
}

void Element::parse(const char*& begin, size_t& offset) {
    key_.keyParse(begin, offset);
    ValueType type = parseValueType(begin, offset);
    v_ = valueFactory(type);
    v_->valueParse(begin, offset);
}

void Element::seri(std::string& result) {
  key_.keySeri(result);
  seriValueType(v_->getType(), result);
  v_->valueSeri(result);
}

Value* Element::getValue() {
  return v_;
}

Element::~Element() {
    delete v_;
}
}
