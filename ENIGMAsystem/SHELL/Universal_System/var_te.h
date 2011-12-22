/**  Copyright (C) 2011 Josh Ventura
**  This file is a part of the ENIGMA Development Environment.
**  It is released under the GNU General Public License, version 3.
**  See the license of local files for more details.
**/

#define unsigl  unsigned long
#define unsigll unsigned long long
#define llong   long long


/** / **
 * / We want error reporting to be unobtrusive, both efficiency-wise and in terms of code space used.
 */

#ifndef __var_te
#define __var_te
  #ifdef DEBUG_MODE
    #define div0c(x) { if (!x) return (show_error("Division by zero.",0), *this); }
    #define terror(x)
    #define nterror(x)
    #define terror2(dt)
    #define terrortrue() show_error("Incompatible right-hand type to operator.",0);
  #else
    #define div0c(x)
    #define terror(x)
    #define terror2(x)
    #define terrortrue()
  #endif

  // Types
  namespace enigma { enum var_types { vt_real, vt_tstr }; }
#endif

#undef EVCONST
#define EVCONST

/** / **
 * / This stuff just takes up entirely too much space.
 */

#define types_extrapolate_real_p(prefix,suffix...)\
 prefix (int x)       EVCONST suffix\
 prefix (long x)      EVCONST suffix\
 prefix (unsigl x)    EVCONST suffix\
 prefix (unsigned x)  EVCONST suffix\
 prefix (long long x) EVCONST suffix\
 prefix (unsigll x)   EVCONST suffix\
 prefix (float x)     EVCONST suffix\
 prefix (double x)    EVCONST suffix

#define types_binary_extrapolate_real_p(prefix,type2,suffix...)\
 prefix (int x,type2 y)       EVCONST suffix\
 prefix (long x,type2 y)      EVCONST suffix\
 prefix (unsigl x,type2 y)    EVCONST suffix\
 prefix (unsigned x,type2 y)  EVCONST suffix\
 prefix (long long x,type2 y) EVCONST suffix\
 prefix (unsigll x,type2 y)   EVCONST suffix\
 prefix (double x,type2 y)    EVCONST suffix\
 prefix (float x,type2 y)     EVCONST suffix

#define types_binary_bitwise_extrapolate_real_p(op,type2,sentiment...)\
 int       operator op(int x,type2 y)       EVCONST { sentiment; return x op int(y); }\
 long      operator op(long x,type2 y)      EVCONST { sentiment; return x op int(y); }\
 unsigned  operator op(unsigned x,type2 y)  EVCONST { sentiment; return x op int(y); }\
 unsigl    operator op(unsigl x,type2 y)    EVCONST { sentiment; return x op int(y); }\
 long long operator op(long long x,type2 y) EVCONST { sentiment; return x op (long long)(y); }\
 unsigll   operator op(unsigll x,type2 y)   EVCONST { sentiment; return x op (unsigll)(y); }\
 long      operator op(double x,type2 y)    EVCONST { sentiment; return long(x) op long(y); }\
 long      operator op(float x,type2 y)     EVCONST { sentiment; return long(x) op long(y); }

#define types_binary_bitwise_extrapolate_alldecc(func, type2)\
 int       func(int x,type2 y) EVCONST;\
 long      func(long x,type2 y) EVCONST;\
 unsigned  func(unsigned x,type2 y) EVCONST;\
 unsigl    func(unsigl x,type2 y) EVCONST;\
 long long func(long long x,type2 y) EVCONST;\
 unsigll   func(unsigll x,type2 y) EVCONST;\
 long      func(double x,type2 y) EVCONST;\
 long      func(float x,type2 y) EVCONST;

#define types_binary_assign_extrapolate_declare(op,type2)\
 int&       operator op##= (int &x,type2 y) EVCONST;\
 long&      operator op##= (long &x,type2 y) EVCONST;\
 unsigl&    operator op##= (unsigl &x,type2 y) EVCONST;\
 unsigned&  operator op##= (unsigned &x,type2 y) EVCONST;\
 long long& operator op##= (long long &x,type2 y) EVCONST;\
 unsigll&   operator op##= (unsigll &x,type2 y) EVCONST;\
 double&    operator op##= (double &x,type2 y) EVCONST;\
 float&     operator op##= (float &x,type2 y) EVCONST;

#define types_binary_assign_extrapolate_implement(op,type2, sentiment...)\
 int&       operator op##= (int &x,type2 y)        EVCONST { sentiment; return x op##= (int)y; }\
 long&      operator op##= (long &x,type2 y)       EVCONST { sentiment; return x op##= (long)y; }\
 unsigl&    operator op##= (unsigl &x,type2 y)     EVCONST { sentiment; return x op##= (long)y; }\
 unsigned&  operator op##= (unsigned &x,type2 y)   EVCONST { sentiment; return x op##= (int)y; }\
 long long& operator op##= (long long &x,type2 y)  EVCONST { sentiment; return x op##= (long long)y; }\
 unsigll&   operator op##= (unsigll &x,type2 y)    EVCONST { sentiment; return x op##= (unsigll)y; }\
 double&    operator op##= (double &x,type2 y)     EVCONST { sentiment; return x op##= (double)y; }\
 float&     operator op##= (float &x,type2 y)      EVCONST { sentiment; return x op##= (float)y; }

#define types_binary_bitwise_assign_extrapolate_implement(op,type2, sentiment...)\
 int&       operator op##= (int &x,type2 y)        EVCONST { sentiment; return x op##= (int)y; }\
 long&      operator op##= (long &x,type2 y)       EVCONST { sentiment; return x op##= (long)y; }\
 unsigl&    operator op##= (unsigl &x,type2 y)     EVCONST { sentiment; return x op##= (long)y; }\
 unsigned&  operator op##= (unsigned &x,type2 y)   EVCONST { sentiment; return x op##= (int)y; }\
 long long& operator op##= (long long &x,type2 y)  EVCONST { sentiment; return x op##= (long long)y; }\
 unsigll&   operator op##= (unsigll &x,type2 y)    EVCONST { sentiment; return x op##= (unsigll)y; }\
 double&    operator op##= (double &x,type2 y)     EVCONST { sentiment; return x = (long)x op (long)y; }\
 float&     operator op##= (float &x,type2 y)      EVCONST { sentiment; return x = (long)x op (long)y; }

#define types_extrapolate_string_p(prefix,suffix...)\
 prefix (const char *x) EVCONST suffix\
 prefix (string x)      EVCONST suffix

#define types_binary_extrapolate_string_p(prefix,type2,suffix...)\
 prefix (const char *x,type2 y) EVCONST suffix\
 prefix (string x,type2 y)      EVCONST suffix

#define types_extrapolate_mix_p(prefix,suffix...)\
 prefix (variant x) EVCONST suffix\
 prefix (var x)     EVCONST suffix

#define types_extrapolate_alldec(prefix)\
 types_extrapolate_real_p  (prefix,;)\
 types_extrapolate_string_p(prefix,;)\
 prefix (const variant x) EVCONST;\
 prefix (const var &x) EVCONST;
#define types_extrapolate_alldecc(prefix)\
 types_extrapolate_real_p  (prefix,;)\
 types_extrapolate_string_p(prefix,;)\
 prefix (const variant &x) EVCONST;\
 prefix (const var &x) EVCONST;
#define types_binary_extrapolate_alldecc(type,fname,type2)\
 types_binary_extrapolate_real_p  (type   fname, type2,;)\
 types_binary_extrapolate_string_p(string fname, type2,;)
#define types_binary_extrapolate_alldecce(type,fname,type2)\
 types_binary_extrapolate_real_p  (type fname, type2,;)\
 types_binary_extrapolate_string_p(type fname, type2,;)

