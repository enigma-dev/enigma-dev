#include "serialization_fwd_decl.h"

template <typename T>
typename std::enable_if<std::is_same_v<bool, std::decay_t<T>>>::type inline enigma::internal_serialize_into_fn(
    std::byte *iter, T &&value) {
  *iter = static_cast<std::byte>(value);
}

inline auto enigma::internal_serialize_fn(bool &&value) {
  return std::vector<std::byte>{static_cast<std::byte>(value)};
}

template <typename T>
typename std::enable_if<std::is_same_v<bool, std::decay_t<T>>, T>::type inline enigma::internal_deserialize_fn(
    std::byte *iter) {
  return static_cast<bool>(*iter);
}
