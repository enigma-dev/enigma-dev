#include "serialization_fwd_decl.h"

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline void enigma::internal_serialize_integral_into(std::byte *iter, T value) {
  internal_serialize_any_into<std::make_unsigned_t<T>>(iter, value);
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline std::array<std::byte, sizeof(T)> enigma::internal_serialize_integral(T value) {
  return serialize_any<std::make_unsigned_t<T>>(value);
}

template <typename T>
inline void enigma::internal_serialize_floating_into(std::byte *iter, T value) {
  if constexpr (std::is_same_v<T, float>) {
    internal_serialize_any_into<std::uint32_t>(iter, value);
  } else if constexpr (std::is_same_v<T, double>) {
    internal_serialize_any_into<std::uint64_t>(iter, value);
  } else {
    static_assert(always_false<T>, "'internal_serialize_floating_into' only accepts 'float' or 'double' types");
  }
}

template <typename T>
inline std::array<std::byte, sizeof(T)> enigma::internal_serialize_floating(T value) {
  if constexpr (std::is_same_v<T, float>) {
    return serialize_any<std::uint32_t>(value);
  } else if constexpr (std::is_same_v<T, double>) {
    return serialize_any<std::uint64_t>(value);
  } else {
    static_assert(always_false<T>, "'internal_serialize_floating' only accepts 'float' or 'double' types");
  }
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T enigma::internal_deserialize_integral(std::byte *iter) {
  return internal_deserialize_any<std::make_unsigned_t<T>, T>(iter);
}

template <typename T>
inline T enigma::internal_deserialize_floating(std::byte *iter) {
  if constexpr (std::is_same_v<T, float>) {
    return internal_deserialize_any<std::uint32_t, T>(iter);
  } else if constexpr (std::is_same_v<T, double>) {
    return internal_deserialize_any<std::size_t, T>(iter);
  } else {
    static_assert(always_false<T>, "'internal_deserialize_floating' only accepts 'float' or 'double' types");
  }
}

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>>::
    type inline enigma::internal_serialize_into_fn(std::byte *iter, T &&value) {
  internal_serialize_numeric_into(iter, value);
}

template <typename T>
typename std::enable_if<std::is_integral_v<std::decay_t<T>> || std::is_floating_point_v<std::decay_t<T>>,
                        std::array<std::byte, sizeof(T)>>::type inline enigma::internal_serialize_fn(T &&value) {
  return internal_serialize_numeric(value);
}

template <typename T>
typename std::enable_if<(std::is_integral_v<std::decay_t<T>> ||
                         std::is_floating_point_v<std::decay_t<T>>)&&!std::is_same_v<std::decay_t<T>, bool>,
                        T>::type inline enigma::internal_deserialize_fn(std::byte *iter) {
  return internal_deserialize_numeric<T>(iter);
}
