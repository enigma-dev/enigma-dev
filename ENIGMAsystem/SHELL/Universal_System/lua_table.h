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

#ifndef _H_LUA_TABLE
#define _H_LUA_TABLE

#include <map>      // Sparse part
#include <string.h> // Memcpy
#include <stdlib.h> // Malloc, Realloc, Free

#ifdef INCLUDED_FROM_SHELLMAIN
#error This file is high-impact and should not be included from SHELLmain.cpp.
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
*/

template <class T> struct lua_table
{
  // This is what kind of sparse container we'll be using
  typedef std::map<size_t,T> lua_map_type;
  typedef typename lua_map_type::iterator shiterator;

  // These are size calculations for the buffer we'll keep
  #define base_size (sizeof(lua_map_type) + sizeof(size_t))
  #define base_length(x) (*(size_t*)((char*)(x) + sizeof(lua_map_type)))
  #define base_map(x)    (*(lua_map_type*)((char*)(x)))
  #define base_to_TA(x) ((T*)((char*)(x) + base_size))

  #define TA_map(x)      (*(lua_map_type*)((char*)(x) - base_size))
  #define TA_length(x)   (*((size_t*)(x) - 1))
  #define TA_start(x)    ((char*)(x) - base_size)
  
  T* dense;
  void initialize()
  {
    // Create our chunk of memory.
    char* databuf = (char*)malloc(base_size + sizeof(T));
    
    // Construct a map for sparse indexes.
    new(databuf) lua_map_type;
    
    base_length(databuf) = 1;  // We'll only allocate one object for now.
    dense = base_to_TA(databuf); // This is where we'll look for it.
    new(dense) T[1]; // Construct it there.
  }
  void move_from_map()
  {
    const size_t clen = TA_length(dense);
    for (shiterator it = TA_map(dense).begin(); it != TA_map(dense).end(); it++)
    {
      if (it->first > clen) break;
      dense[it->first] = it->second;
      TA_map(dense).erase(it++);
    }
  }
  inline void destroy()
  {
    // Iterate the dense part, destroying everything.
    const size_t dlen = TA_length(dense);
    for (size_t i = 0; i < dlen; i++)
      dense[i].~T();
    
    // Destroy the map.
    TA_map(dense).~lua_map_type();
    
    // Give back the memory.
    free(TA_start(dense));
    dense = NULL;
  }
  
  void pick_up(const lua_table<T>& who)
  {
    const size_t len = TA_length(who.dense);
    
    // Create a new chunk, base and all. We'll be moving here.
    char* databuf = (char*)malloc(base_size + len*sizeof(T));
    
    // Call map copy constructor.
    new(databuf) lua_map_type(TA_map(who.dense));
    
    base_length(databuf) = len;   // We share a length in common, though.... 
    T* ndense = base_to_TA(databuf); // Re-establish our array's location.
    for (size_t i=0; i<len; i++) // Copy the array elements
      new(ndense+i) T(who.dense[i]);
    if (dense) destroy();
    dense = ndense;
  }
  void upsize(const size_t c)
  {
    const size_t dense_size = TA_length(dense);
    
    // Preserve our map
    lua_map_type tmp; // Create a temporary map
    tmp.swap(TA_map(dense)); // Move our old map into it
    TA_map(dense).~lua_map_type(); // Destroy our old map
    
    char* databuf = TA_start(dense); // Get the beginning of the data chunk
    databuf = (char*)realloc(databuf,base_size + c*sizeof(T));   // Ask system to give us more memory and maybe move us
    
    // Restore our map
    new(databuf) lua_map_type(); // Recreate our map
    lua_map_type& nmap = base_map(databuf); // Reference our new map
    nmap.swap(tmp); // Move our old content into it
    // I know this looks like tail-chasing, but map<> WILL segfault otherwise. I can't tell why.
    
    // Finish our move
    dense = base_to_TA(databuf);   // Get back our array pointer
    new(dense + dense_size) T[c - dense_size]; // Construct new array elements
    
    for (shiterator i = nmap.begin(); i != nmap.end(); ) {
      if (i->first >= c) break;
      dense[i->first] = i->second;
      nmap.erase(i++);
    }
    TA_length(dense) = c;    // Store new alloc size for dense array
  }
  
  T& operator[] (size_t ind) 
  { 
    const size_t dense_size = TA_length(dense);
    if (ind >= dense_size)
    {
      // Calculate tolerable size increase
      const size_t c = (dense_size < 4) ? 4 : dense_size << 1;
      
      // If we're far out of range of the currently allocated dense array
      if (ind >= c) // Default to the map and return a sparse node
        return TA_map(dense)[ind];
      
      // Otherwise, resize and reconstruct
      upsize(c);
    }
    return dense[ind];
  }
  T& operator*() {
    return *dense;
  }
  
  lua_table<T>& operator= (const lua_table<T>& x)
  {
    pick_up(x);
    return *this;
  }
  
  lua_table<T>() {
    initialize();
  }
  lua_table<T>(const lua_table<T> &x): dense(NULL) {
    pick_up(x);
  }
  ~lua_table<T>() {
    destroy();
  }
};

#endif //_H_LUA_TABLE
