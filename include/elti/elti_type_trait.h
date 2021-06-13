#pragma once

namespace elti {
namespace detail {
template<typename T>
struct NoRefNoPointer {
  static constexpr bool value = true;
};

template<typename T>
struct NoRefNoPointer<T&> {
  static constexpr bool value = false;
};

template<typename T>
struct NoRefNoPointer<T&&> {
  static constexpr bool value = false;
};

template<typename T>
struct NoRefNoPointer<T*> {
  static constexpr bool value = false;
};

template<typename T>
struct NoRefNoPointer<T* const> {
  static constexpr bool value = false;
};
}
}
