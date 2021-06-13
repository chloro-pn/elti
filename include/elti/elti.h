#pragma once
#include "elti/element.h"
#include "elti/value.h"
#include "elti/value_wrapper.h"
#include "elti/positioner.h"
#include "elti/elti_enum.h"
#include <memory>

namespace elti {
class Elti {
private:
  Element root_;
  ParseRef ref_;

  Value* getValue() {
    return root_.getValue();
  }

  const Value* getValue() const {
      return root_.getValue();
  }

public:
  static std::pair<size_t, Elti> parseToElti(const char* ptr, ParseRef ref = ParseRef::Off) {
    Elti r(ref);
    size_t offset = r.parse(ptr);
    return std::make_pair(offset, std::move(r));
  }

  static Positioner parseToPositioner(const char* ptr) {
    Key key("");
    size_t offset = 0;
    key.keyParse(ptr, offset);
    ValueType type = parseValueType(ptr, offset);
    return Positioner(ptr, type);
  }

  //外部seri所用构造函数
  explicit Elti(std::unique_ptr<Value>&& v) : root_("/", std::move(v)), ref_(ParseRef::Off) {

  }

private:
  //内部parse所用的构造函数
  explicit Elti(ParseRef ref):root_(), ref_(ref) {}

public:
  Elti(Elti&&) = default;
  Elti& operator=(Elti&&) = default;
  Elti(const Elti&) = delete;
  Elti& operator=(const Elti&) = delete;

  std::unique_ptr<Value> reset() {
    std::unique_ptr<Value> tmp = std::move(root_.v_);
    root_.v_ = nullptr;
    return tmp;
  }

  void seri(std::string& result) const {
    root_.seri(result);
  }

  std::string seriToString() const {
      std::string result;
      root_.seri(result);
      return result;
  }

  size_t parse(const char* ptr) {
    size_t offset = 0;
    root_.parse(ptr, offset, ref_);
    return offset;
  }

  ValueWrapper getRoot() const {
    return ValueWrapper(root_.v_.get(), ref_);
  }
};
}
