/**  Copyright (C) 2008 Josh Ventura
**  This file is a part of the ENIGMA Development Environment.
**  It is released under the GNU General Public License, version 3.
**  See the license of local files for more details.
**/

#define unsigll unsigned long long


/** / **
 * / We want error reporting to be unobtrusive, both efficiency-wise and in terms of code space used.
 */

#ifndef __var_te
#define __var_te
  #ifdef DEBUGMODE
    #define div0c(x) if (!x) return show_error("Division by zero.",0), 0;
    #define terror(x) if (type == x) { show_error("Incompatible types to operator.",0); }
    #define terror2(dt) if (type == dt or x.type == dt) { show_error("Incompatible types to operator.",0); }
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


/** / **
 * / This stuff just takes up entirely too much space.
 */
 
#define types_extrapolate_real_p(prefix,suffix...)\
 prefix (int x)       suffix\
 prefix (unsigned x)  suffix\
 prefix (long long x) suffix\
 prefix (unsigll x)   suffix\
 prefix (float x)     suffix\
 prefix (double x)    suffix

#define types_binary_extrapolate_real_p(prefix,type2,suffix...)\
 prefix (int x,type2 y)       suffix\
 prefix (unsigned x,type2 y)  suffix\
 prefix (long long x,type2 y) suffix\
 prefix (unsigll x,type2 y)   suffix\
 prefix (double x,type2 y)    suffix\
 prefix (float x,type2 y)     suffix
 
#define types_binary_bitwise_extrapolate_real_p(op,type2,sentiment...)\
 int       operator op(int x,type2 y)       { sentiment; return x op int(y); }\
 unsigned  operator op(unsigned x,type2 y)  { sentiment; return x op int(y); }\
 long long operator op(long long x,type2 y) { sentiment; return x op (long long)(y); }\
 unsigll   operator op(unsigll x,type2 y)   { sentiment; return x op (unsigll)(y); }\
 long      operator op(double x,type2 y)    { sentiment; return long(x) op long(y); }\
 long      operator op(float x,type2 y)     { sentiment; return long(x) op long(y); }
 
#define types_binary_bitwise_extrapolate_alldecc(func, type2)\
 int       func(int x,type2 y);\
 unsigned  func(unsigned x,type2 y);\
 long long func(long long x,type2 y);\
 unsigll   func(unsigll x,type2 y);\
 long      func(double x,type2 y);\
 long      func(float x,type2 y);
 
#define types_binary_assign_extrapolate_declare(op,type2)\
 int&       operator op##= (int &x,type2 y);\
 unsigned&  operator op##= (unsigned &x,type2 y);\
 long long& operator op##= (long long &x,type2 y);\
 unsigll&   operator op##= (unsigll &x,type2 y);\
 double&    operator op##= (double &x,type2 y);\
 float&     operator op##= (float &x,type2 y);
 
#define types_binary_assign_extrapolate_implement(op,type2, sentiment...)\
 int&       operator op##= (int &x,type2 y)        { sentiment; return x op##= (int)y; }\
 unsigned&  operator op##= (unsigned &x,type2 y)   { sentiment; return x op##= (int)y; }\
 long long& operator op##= (long long &x,type2 y)  { sentiment; return x op##= (long long)y; }\
 unsigll&   operator op##= (unsigll &x,type2 y)    { sentiment; return x op##= (unsigll)y; }\
 double&    operator op##= (double &x,type2 y)     { sentiment; return x op##= (double)y; }\
 float&     operator op##= (float &x,type2 y)      { sentiment; return x op##= (float)y; }
 
#define types_binary_bitwise_assign_extrapolate_implement(op,type2, sentiment...)\
 int&       operator op##= (int &x,type2 y)        { sentiment; return x op##= (int)y; }\
 unsigned&  operator op##= (unsigned &x,type2 y)   { sentiment; return x op##= (int)y; }\
 long long& operator op##= (long long &x,type2 y)  { sentiment; return x op##= (long long)y; }\
 unsigll&   operator op##= (unsigll &x,type2 y)    { sentiment; return x op##= (unsigll)y; }\
 double&    operator op##= (double &x,type2 y)     { sentiment; return x = (long)x op (long)y; }\
 float&     operator op##= (float &x,type2 y)      { sentiment; return x = (long)x op (long)y; }

#define types_extrapolate_string_p(prefix,suffix...)\
 prefix (const char *x) suffix\
 prefix (string x)      suffix
 
#define types_binary_extrapolate_string_p(prefix,type2,suffix...)\
 prefix (const char *x,type2 y) suffix\
 prefix (string x,type2 y)      suffix
 
#define types_extrapolate_mix_p(prefix,suffix...)\
 prefix (variant x) suffix\
 prefix (var x)     suffix

#define types_extrapolate_alldec(prefix)\
 types_extrapolate_real_p  (prefix,;)\
 types_extrapolate_string_p(prefix,;)\
 prefix (const variant x);\
 prefix (const var &x);
#define types_extrapolate_alldecc(prefix)\
 types_extrapolate_real_p  (prefix,;)\
 types_extrapolate_string_p(prefix,;)\
 prefix (const variant &x);\
 prefix (const var &x);
#define types_binary_extrapolate_alldecc(type,fname,type2)\
 types_binary_extrapolate_real_p  (type   fname, type2,;)\
 types_binary_extrapolate_string_p(string fname, type2,;)
#define types_binary_extrapolate_alldecce(type,fname,type2)\
 types_binary_extrapolate_real_p  (type fname, type2,;)\
 types_binary_extrapolate_string_p(type fname, type2,;)

