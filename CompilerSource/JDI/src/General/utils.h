#ifndef JDI_UTILS_h
#define JDI_UTILS_h

#include <memory>

namespace jdi {

template<typename T, typename U>
std::unique_ptr<T> cast_unique(std::unique_ptr<U> p) {
  return std::unique_ptr<T>((T*) p.release());
}

}  // namespace jdi

#endif
