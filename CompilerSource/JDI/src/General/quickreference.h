/**
 * @file  quickreference.h
 * @brief Header meant to beautify code using a pointer to a pointer.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2013 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _QUICKREFERENCE__H
#define _QUICKREFERENCE__H

namespace quick
{
  /// Class meant to beautify code using a pointer to a pointer.
  template<typename tp> struct double_pointer {
    tp **refd;
    inline tp *operator->() const { return *refd; }
    inline tp &operator*() const { return **refd; }
    inline double_pointer<tp> &operator=(tp* x) { *refd = x; return *this; }
    
    inline operator tp*() { return *refd; }
    template<typename tpto> inline operator tpto*() { return (tpto*)*refd; }
    
    /// Single dereference: return the actual pointer. The ugly hack to make up for how pleasant this looks, otherwise.
    inline tp* operator~() { return *refd; }
    
    inline bool operator==(tp *x) { return *refd == x; }
    inline bool operator!=(tp *x) { return *refd != x; }
    inline bool operator>=(tp *x) { return *refd >= x; }
    inline bool operator<=(tp *x) { return *refd <= x; }
    inline bool operator> (tp *x) { return *refd >  x; }
    inline bool operator< (tp *x) { return *refd <  x; }
    
    double_pointer(tp** to_ref): refd(to_ref) {}
  };
}

#endif
