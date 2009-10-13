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

#include "expression_evaluator_settings.h"

#define RTYPE_NONE -1
#define RTYPE_INT 0
#define RTYPE_DOUBLE 1
#define RTYPE_STRING 2
#define RTYPE_UINT 3

struct value
{
  union 
  {
    #if USETYPE_INT
      UTYPE_INT i;
      #define ZEROI(x) x real.i=0;
    #else
      #define ZEROI(x)
    #endif
    
    #if USETYPE_DOUBLE
      UTYPE_DOUBLE d; 
      #define ZEROD(x) x real.d=0;
    #else
      #define ZEROD(x)
    #endif
    
    #if USETYPE_UINT
      UTYPE_UINT u;
      #define ZEROU(x) x real.u=0;
    #else
      #define ZEROU(x)
    #endif
  } real;
  
  #if USETYPE_STRING
    string str;
    #define ZEROS(x) x str="";
    #else
      #define ZEROS(x)
  #endif
  int type;
  
  #define VZERO(x) { ZEROI(x) ZEROD(x) ZEROU(x) ZEROS(x) }
  
  value()
  {
    type=0;
    VZERO();
  }
  value(const value &a)
  {
    VZERO();
    type=a.type;
    #if USETYPE_STRING
      if (type==RTYPE_STRING)
        str=a.str;
      else
    #endif
    #if USETYPE_INT
      if (type==RTYPE_INT)
        real.i=a.real.i;
      else
    #endif
    #if USETYPE_DOUBLE
      if (type==RTYPE_DOUBLE)
        real.d=a.real.d;
      else
    #endif
    #if USETYPE_UINT
      if (type==RTYPE_UINT)
        real.u=a.real.u;
      else
    #endif
    ;
  }
  value& operator=(value a)
  {
    type=a.type;
    #if USETYPE_STRING
      if (type==RTYPE_STRING)
        str=a.str;
      else
    #endif
    #if USETYPE_INT
      if (type==RTYPE_INT)
        real.i=a.real.i;
      else
    #endif
    #if USETYPE_DOUBLE
      if (type==RTYPE_DOUBLE)
        real.d=a.real.d;
      else
    #endif
    #if USETYPE_UINT
      if (type==RTYPE_UINT)
        real.u=a.real.u;
      else
    #endif
    ; return *this;
  }
  
  value(UTYPE_INT a)
  {
    VZERO();
    #if USETYPE_INT
      real.i=a;
      type=RTYPE_INT;
    #elif USETYPE_UINT
      real.u=a;
      type=RTYPE_UINT;
    #elif USETYPE_DOUBLE
      real.d=a;
      type=RTYPE_DOUBLE;
    #endif
  }
  
  value(int a)
  {
    VZERO();
    #if USETYPE_INT
      real.i=a;
      type=RTYPE_INT;
    #elif USETYPE_UINT
      real.u=a;
      type=RTYPE_UINT;
    #elif USETYPE_DOUBLE
      real.d=a;
      type=RTYPE_DOUBLE;
    #endif
  }
  
  #if USETYPE_UINT
    value(UTYPE_UINT a)
    {
      VZERO();
      real.u=a;
      type=RTYPE_UINT;
    }
  #endif
  
  
  
  operator UTYPE_INT()
  {
    #if USETYPE_INT
      if (type==RTYPE_INT)
      return real.i;
    #endif
    #if USETYPE_DOUBLE
      if (type==RTYPE_DOUBLE)
      return (UTYPE_INT)(real.d);
    #endif
    #if USETYPE_UINT
      if (type==RTYPE_UINT)
      return (UTYPE_INT)(real.u);
    #endif
    return 0;
  }
  
  #if USETYPE_DOUBLE
    value(UTYPE_DOUBLE a)
    {
      VZERO();
      real.d=a;
      type=RTYPE_DOUBLE;
    }
    operator UTYPE_DOUBLE()
    {
      #if USETYPE_INT
        if (type==RTYPE_INT) return real.i;
      #endif
      #if USETYPE_DOUBLE
        if (type==RTYPE_DOUBLE) return real.d;
      #endif
      #if USETYPE_UINT
        if (type==RTYPE_UINT) return real.u;
      #endif
      return 0;
    }
  #endif
  
  operator bool()
  {
    #if USETYPE_INT
      if (type==RTYPE_INT) return real.i;
    #endif
    #if USETYPE_DOUBLE
      if (type==RTYPE_DOUBLE) return real.d;
    #endif
    #if USETYPE_UINT
      if (type==RTYPE_UINT) return real.u;
    #endif
    return 0;
  }
  
  #if USETYPE_STRING
    value(string a)
    {
      VZERO();
      type=RTYPE_STRING;
      str=a;
    }
    operator string() { if (type==RTYPE_STRING) return str; return ""; }
  #endif
  
};
