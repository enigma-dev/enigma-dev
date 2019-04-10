/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#ifndef ENIGMA_H_LUA_TABLE
#define ENIGMA_H_LUA_TABLE

#ifndef JUST_DEFINE_IT_RUN
#include <map>     // Sparse part
#include <vector>  // Dense part
#include <cstring> // Memcpy
#include <cstdlib> // Malloc, Realloc, Free
#endif

/**
  This file implements a Lua-table-like structure. It borrows ideas not only from
  Lua, but from STL containers. It also borrows the entirety of std::map. This should
  be replaced with a sparse hash map container when it's a "good time" to do so.

  By ENIGMA-defined standard, this table class takes up sizeof(void*) bytes. The class
  itself contains a single pointer to a dense part (dynamic array). This ctually points
  to a fixed-length position from the beginning of an allocated block. At the beginning
  of this block is precisely enough room for the table's map component. The implication
  is that the dense segment can be dereferenced without additional arithmetic (by unary
  operator*), while the map section requires taking dense[-sizeof(map)],  where `dense`
  has been cast to char*, then casting back to maptype*.

  NOTE: Memory layout is something like this:
    <sparse_map><dense_length><max_elem+1><dense_elem[0],dense_elem[1],...>
*/

namespace {
template <typename T>
T my_max(const T& lhs, const T& rhs) {
  return lhs>rhs ? lhs : rhs;
}
}


template <class T> struct lua_table {
# ifndef JUST_DEFINE_IT_RUN
  typedef std::vector<T> dense_type;
  typedef std::map<size_t,T> sparse_type;
# else
  typedef T dense_type[1];
  typedef T sparse_type[1];
# endif

 private:
  dense_type dense;
  sparse_type sparse;

  // Our one item of book-keeping
  size_t mx_size;  ///< highest accessed element + 1


  void pick_up(const lua_table<T>& who) {
    mx_size = who.mx_size;
    dense = who.dense;
    sparse = who.sparse;
  }

  void upsize(const size_t c)
  {
    dense.resize(c);
    mx_size = c;
    //Copy sparse array values that are now within this reserve space.
    for (typename sparse_type::iterator it=sparse.begin(); it!=sparse.end();) {
      if (it->first >= c) {
        break;
      }
      dense[it->first] = it->second;
      sparse.erase(it++);
    }
  }


 public:
  const dense_type &dense_part() const {
    return dense;
  }

  T& operator[] (size_t ind) {
    mx_size = my_max(ind+1, mx_size);
    if (ind >= dense.size()) {
      size_t nsize = my_max(dense.size() << 1, dense.capacity());
      if (ind >= nsize) {
        return sparse[ind];
      }
      upsize(ind + 1);
    }
    return dense[ind];
  }

  const T& operator[] (size_t ind) const {
    static const T sentinel;
    if (ind >= dense.size()) {
      auto f = sparse.find(ind);
      return f == sparse.end()? sentinel : f->second;
    }
    return dense[ind];
  }

  void fill(const T &value, size_t length) {
    upsize(length);
    for (size_t i = 0; i < length; ++i)
      dense[i] = value;
  }

  T& operator*() {
    return dense[0];
  }

  const T& operator*() const {
    return dense[0];
  }

  int max_index() const {
    return mx_size;
  }

  size_t dense_length() const {
    return dense.size();
  }

  lua_table<T>& operator= (const lua_table<T>& x) {
    pick_up(x);
    return *this;
  }

  lua_table<T>(): mx_size(0) {}
  lua_table<T>(const T &v, size_t len): dense(len, v), mx_size(len) {}
  lua_table<T>(const lua_table<T> &x) {
    pick_up(x);
  }
  ~lua_table<T>() {}
};

#endif // ENIGMA_H_LUA_TABLE
