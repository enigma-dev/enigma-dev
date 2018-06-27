#ifndef ENIGMA_RESOURCVEC_H
#define ENIGMA_RESOURCVEC_H

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"
#include "libEGMstd.h"
#endif

#include "make_unique.h"

#include <memory>
#include <string>
#include <vector>

namespace enigma {

template <class T>
class ResourceVec {
 public:
  ResourceVec(std::string type) : type(type) {}
  T* operator[](int index) {
#ifdef DEBUG_MODE
    if (!exists(index)) {
      show_error("Attempting to access non-existing " + toString(type) + " " + toString(index), false);
      return nullptr;
    }
#endif

    return res[index].get();
  }

  void assign(int index, std::unique_ptr<T> r) { res[index] = std::move(r); }
  void push_back(std::unique_ptr<T> r) { res.push_back(std::move(r)); }
  void pop_back() { res.pop_back(); }
  bool exists(int index) { return index >= 0 && index < static_cast<int>(res.size()) && res[index] != nullptr; }
  size_t size() { return res.size(); }
  void remove(int index) {
#ifdef DEBUG_MODE
    if (!exists(index)) {
      show_error("Attempting to delete non-existing " + toString(type) + " " + toString(index), false);
      return;
    }
#endif

    res[index] = nullptr;
  }
  void duplicate(int index) {
    res.push_back(make_unique<T>(*res[index]));
  }

 protected:
  std::vector<std::unique_ptr<T>> res;
  std::string type;
};

#define get_res(vec, index, res) res = vec[index];

#ifdef DEBUG_MODE
#define get_resi(vec, index, res) get_res(vec, index, res) if (res == nullptr) return -1;
#define get_resv(vec, index, res) get_res(vec, index, res) if (res == nullptr) return;
#define get_resb(vec, index, res) get_res(vec, index, res) if (res == nullptr) return false;
#else
#define get_resi(vec, index, res) get_res(vec, index, res);
#define get_resv(vec, index, res) get_res(vec, index, res);
#define get_resb(vec, index, res) get_res(vec, index, res);
#endif

}  // namespace enigma

#endif //ENIGMA_RESOURCVEC_H
