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

#ifdef INCLUDED_FROM_SHELLMAIN
#error This file is high-impact and should not be included from SHELLmain.cpp.
#endif

#ifndef ENIGMA_H_LUA_TABLE
#define ENIGMA_H_LUA_TABLE

#include <map>      // Sparse part
#include <string.h> // Memcpy
#include <stdlib.h> // Malloc, Realloc, Free

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


template <class T> struct lua_table
{
  // This is what kind of sparse container we'll be using
  typedef std::map<size_t,T> lua_map_type;

private:
  //Stuff relating to dense storage.
  T* dense;
  size_t dn_reserve; //Total available units.

  //Sparse storage is easier.
  lua_map_type sparse;

  //Our one item of book-keeping
  size_t mx_size; //Actual max_element+1


  void pick_up(const lua_table<T>& who)
  {
    dn_reserve = who.dn_reserve;
    mx_size = who.mx_size;

    //Free old memory?
    if (dense) {
      delete [] dense;
      dense = NULL;
    }

    //Create a new dense chunk, copy over values.
    dense = new T[dn_reserve];
    for (size_t i=0; i<dn_reserve; i++) {
      dense[i] = who.dense[i];
    }

    //Copy the sparse map
    sparse = who.sparse;
  }

  void upsize(const size_t c)
  {
    //Create a new dense section and copy over values; free old memory.
    T* new_dense = new T[c];
    if (dense) {
      for (size_t i=0; i<dn_reserve; i++) {
        new_dense[i] = dense[i];
      }
      delete [] dense;
    }
    dense = new_dense;
    dn_reserve = c;

    //Copy sparse array values that are now within this reserve space.
    for (typename lua_map_type::iterator it=sparse.begin(); it!=sparse.end();) {
      if (it->first >= c) {
        break;
      }
      dense[it->first] = it->second;
      sparse.erase(it++);
    }
  }


public:
  T& operator[] (size_t ind) {
    mx_size = my_max(ind+1, mx_size);
    if (ind >= dn_reserve) {
      // Calculate tolerable size increase
      const size_t c = (dn_reserve < 4) ? 4 : dn_reserve << 1;

      // If we're far out of range of the currently allocated dense array
      if (ind >= c) // Default to the map and return a sparse node
        return sparse[ind];

      // Otherwise, resize and reconstruct
      upsize(c);
    }

    return dense[ind];
  }
  
  const T& operator[] (size_t ind) const {
    static const T sentinel;
    if (ind >= dn_reserve) {
      auto f = sparse.find(ind);
      return f == sparse.end()? sentinel : f->second;
    }
    return dense[ind];
  }

  T& operator*() {
    return dense[0];
  }

  int max_index() const {
    return mx_size;
  }

  lua_table<T>& operator= (const lua_table<T>& x)
  {
    pick_up(x);
    return *this;
  }

  lua_table<T>() : dense(new T[1]), dn_reserve(1), mx_size(1) {
  }
  lua_table<T>(const lua_table<T> &x): dense(NULL), dn_reserve(0), mx_size(0) {
    pick_up(x);
  }
  ~lua_table<T>() {
    if (dense) { delete [] dense; }
  }
};

#endif // ENIGMA_H_LUA_TABLE
