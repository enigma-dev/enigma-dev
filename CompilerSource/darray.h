/*********************************************************************************\
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
\*********************************************************************************/

#include <string.h>

template <typename atype>
struct darray
{
  atype* where;
  atype safety_val;
  unsigned int size;
  unsigned int allocd;
  atype &operator[] (unsigned int ind)
  {
    if (allocd<=ind)
    {
      int olds=allocd;
      while (allocd<=ind) allocd<<=1;

      atype* nar=new atype[allocd];
      if (nar==0)
      {
         allocd=olds;
         return safety_val;
      }
      if (olds>0)
        memcpy(nar,where,olds*sizeof(atype));

      delete []where;
      where=nar;
    }
    if (ind+1>size) size=ind+1;
    return where[ind];
  }
  darray() { where=new atype[1]; allocd=(where!=0); size=0; }
  ~darray() { if (where != 0) delete[] where; }
};

template <typename atype>
struct varray
{
  atype** where;
  atype* safety_val;
  unsigned int size;
  unsigned int allocd;
  atype &operator[] (unsigned int ind)
  {
    if (allocd<=ind)
    {
      int olds=allocd;
      while (allocd<=ind) allocd<<=1;

      atype** nar=new atype*[allocd];
      if (nar==0)
      {
         allocd=olds;
         return *safety_val;
      }
      if (olds>0)
      {
        memcpy(nar,where,olds*sizeof(atype*));
        for (unsigned int i=olds;i<allocd;i++) nar[i]=0;
      }

      delete []where;
      where=nar;
    }
    if (ind+1>size) size=ind+1;
    if (where[ind]==0) where[ind]=new atype;
    if (where[ind]==0) return *safety_val;
    return *where[ind];
  }
  varray() { safety_val=new atype; where=new atype*[1]; allocd=(where!=0); if (allocd==0 or safety_val==0) exit(-18); where[0]=0; size=0; }
  ~varray() { if (where != 0) for (unsigned int i=0; i<size; i++) delete where[i]; }
};
