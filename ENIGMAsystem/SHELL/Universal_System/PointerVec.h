#ifndef ENIGMA_POINTER_VEC
#define ENIGMA_POINTER_VEC

#include <vector>

template <class T>
class PointerVec : public std::vector<T> {
public:
  PointerVec() : std::vector<T>() {}
  PointerVec(size_t size) : std::vector<T>(size) {}
  
  ~PointerVec() {
     for (size_t i = 0; i < std::vector<T>::size(); i++) {
        if (std::vector<T>::at(i) != nullptr) 
          delete std::vector<T>::at(i);
     }
  }
};

#endif
