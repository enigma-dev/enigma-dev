#include "serialization_fwd_decl.h"

template <typename T>
inline auto enigma::internal_serialize_into_fn(std::byte *iter, T *value) {
  internal_serialize_into<std::size_t>(iter, 0);
}

template <typename T>
inline auto enigma::internal_serialize_fn(T *&&value) {
  return internal_serialize_numeric<std::size_t>(0);
}

template <typename T>
typename std::enable_if<std::is_pointer_v<std::decay_t<T>>, T>::type inline enigma::internal_deserialize_fn(std::byte *iter) {
  return nullptr;
}
