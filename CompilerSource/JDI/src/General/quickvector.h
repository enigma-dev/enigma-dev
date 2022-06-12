/**
 * @file  quickvector.h
 * @brief A file implementing a small, fast vector type with a swap_on method.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _QUICKVECTOR__H
#define _QUICKVECTOR__H

#include <cstdlib>
#include <General/debug_macros.h>

/** Namespace holding specialized container data types designed to up speed or minimize allocation.
**/
namespace quick {
  /** A basic stack structure designed to minimize allocation.
  **/
  template<typename tp, int chunksize=2> class vector
  {
    tp **data;
    tp *statics[chunksize];
    unsigned mysize;
    unsigned reserved;
  public:
    size_t size() const { return mysize; }
    tp &operator[](unsigned ind) { dbg_assert(ind < mysize); return *data[ind]; }
    const tp &operator[](unsigned ind) const { dbg_assert(ind < mysize); return *data[ind]; }
    
    void swap(quick::vector<tp,chunksize> &vec)
    {
      size_t ssw = vec.mysize;
      vec.mysize = mysize;
      mysize = ssw;
      ssw = vec.reserved;
      vec.reserved = reserved;
      reserved = ssw;
      
      if (reserved <= chunksize) {
        data = (tp**)&statics;
        
        if (vec.reserved < chunksize) {
          tp *dsw;
          vec.data = (tp**)&vec.statics;
          for (int i = 0; i < chunksize; i++)
            dsw = statics[i], statics[i] = vec.statics[i], vec.statics[i] = dsw;
        }
        else for (int i = 0; i < chunksize; i++)
          statics[i] = vec.statics[i];
      }
      else if (vec.reserved < chunksize) {
        vec.data = (tp**)&vec.statics;
        for (int i = 0; i < chunksize; i++)
          vec.statics[i] = statics[i];
      }
      else {
        tp **dsw = data;
        data = vec.data;
        vec.data = dsw;
      }
    }
    
    void enswap(tp &member)
    {
      if (mysize < reserved) 
      {
        if (reserved <= chunksize) {
          data = (tp**)malloc(sizeof(tp*)*(reserved += chunksize));
          for (int i = 0; i < chunksize; i++) data[i] = statics[i];
        }
        else
          data = (tp**)realloc(data,sizeof(tp*)*(reserved += chunksize));
      }
      data[mysize] = new tp();
      data[mysize++]->swap(member);
    }
    
    vector(): data((tp**)&statics), mysize(0), reserved(chunksize) {}
    ~vector() {
      for (size_t i = 0; i < mysize; i++) delete data[i];
      if (reserved > chunksize) free(data);
    }
  };
}

#endif
