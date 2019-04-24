/** Copyright (C) 2011 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/


#include "var4.h"
#undef   string
#include "var_te.h"

#include <math.h>

using std::string;

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"
  #define ccast(tpc) { if (type != tpc) \
    { if (type==-1) show_error("Accessing uninitialized variable",0); \
      else show_error(string("Attempting to access ") + (type==0?"real":type==1?"string":"pointer")\
                      + " variable as " + (tpc==0?"real":tpc==1?"string":"pointer"),0); } }
#else
  #define ccast(disregard)
#endif

#ifdef CODEBLOX
#  define codebloxt(x, y) (x)
#else
#  define codebloxt(x, y) (y)
#endif

template<typename t> inline bool vareq(double x, t y) { return fabs(x - y) <= 1e-12; }
template<>           inline bool vareq(double x, double y) { return fabs(x - y) < 1e-12; }
template<>           inline bool vareq(double x, float y)  { return fabsf(float(x) - y) < 1e-8; }
template<typename t> inline bool varneq(double x, t y) { return fabs(x - y) > 1e-12; }
template<>           inline bool varneq(double x, double y) { return fabs(x - y) > 1e-12; }
template<>           inline bool varneq(double x, float y)  { return fabsf(float(x) - y) >= 1e-8; }
template<typename t> inline bool varzero(t x)      { return !x; }
template<>           inline bool varzero(float x)  { return codebloxt(x >= 0 && x <= 0, !x); }
template<>           inline bool varzero(double x) { return codebloxt(x >= 0 && x <= 0, !x); }

#if DEBUG_MODE
#  define div0c(x) { if (varzero(x)) return (show_error("Division by zero.",0), *this); }
#else
#  define div0c(x)
#endif

namespace enigma_user {

variant::operator int()       { ccast(0); return int  (rval.d); }
variant::operator bool()      { ccast(0); return lrint(rval.d) > 0; }
variant::operator char()      { ccast(0); return char (rval.d); }
variant::operator long()      { ccast(0); return long (rval.d); }
variant::operator short()     { ccast(0); return short(rval.d); }
variant::operator unsigned()           { ccast(0); return (unsigned int)       (rval.d); }
variant::operator unsigned char()      { ccast(0); return (unsigned char)      (rval.d); }
variant::operator unsigned short()     { ccast(0); return (unsigned short)     (rval.d); }
variant::operator unsigned long()      { ccast(0); return (unsigned long) (rval.d); }
variant::operator unsigned long long() { ccast(0); return (unsigned long long) (rval.d); }
variant::operator long long() { ccast(0); return (long long)(rval.d); }
variant::operator double()    { ccast(0); return double     (rval.d); }
variant::operator float()     { ccast(0); return float      (rval.d); }

variant::operator string() { ccast(1); return sval; }

variant::operator int()       const { ccast(0); return int  (rval.d); }
variant::operator bool()      const { ccast(0); return lrint(rval.d) > 0; }
variant::operator char()      const { ccast(0); return char (rval.d); }
variant::operator long()      const { ccast(0); return long (rval.d); }
variant::operator short()     const { ccast(0); return short(rval.d); }
variant::operator unsigned()           const { ccast(0); return (unsigned int)       (rval.d); }
variant::operator unsigned char()      const { ccast(0); return (unsigned char)      (rval.d); }
variant::operator unsigned short()     const { ccast(0); return (unsigned short)     (rval.d); }
variant::operator unsigned long()      const { ccast(0); return (unsigned long) (rval.d); }
variant::operator unsigned long long() const { ccast(0); return (unsigned long long) (rval.d); }
variant::operator long long() const { ccast(0); return (long long)(rval.d); }
variant::operator double()    const { ccast(0); return double     (rval.d); }
variant::operator float()     const { ccast(0); return float      (rval.d); }

variant::operator string() const { ccast(1); return sval; }

#define real enigma_user::ty_real
#define tstr enigma_user::ty_string

types_extrapolate_real_p  (variant::variant,: rval(x), sval( ), type(real) {})
types_extrapolate_string_p(variant::variant,: rval(0.0), sval(x), type(tstr) {})
//variant::variant(var x): rval(x[0].rval), sval(x[0].sval) { }
variant::variant(const void *p): rval(p), type(enigma_user::ty_pointer) {}
variant::variant(const variant& x): rval(x.rval.d), sval(x.sval), type(x.type) { }
variant::variant(const var& x): rval((*x).rval.d), sval((*x).sval), type((*x).type) { }
variant::variant(): rval(0.0), sval( ), type(default_type) { }

types_extrapolate_real_p  (variant& variant::operator=, { rval.d = x; type = real; return *this; })
types_extrapolate_string_p(variant& variant::operator=, { sval   = x; type = tstr; return *this; })
variant& variant::operator=(const variant x)            { rval.d = x.rval.d; if ((type = x.type) == tstr) sval = x.sval; return *this; }
variant& variant::operator=(const var &x)               { return *this = *x; }
variant& variant::operator=(const void* p)              { type = enigma_user::ty_pointer; rval.p = p; return *this; }

types_extrapolate_real_p  (variant& variant::operator+=, { terror(real); rval.d += x; return *this; })
types_extrapolate_string_p(variant& variant::operator+=, { terror(tstr); sval   += x; return *this; })
variant& variant::operator+=(const variant x)            { terror(x.type); if (x.type == real) rval.d += x.rval.d; else sval += x.sval; return *this; }
variant& variant::operator+=(const var &x)               { return *this += *x; }

types_extrapolate_real_p  (variant& variant::operator-=, { terror(real); rval.d -= x; return *this; })
types_extrapolate_string_p(variant& variant::operator-=, { terrortrue(); return *this; })
variant& variant::operator-=(const variant x)            { terror2(real); rval.d -= x.rval.d; return *this; }
variant& variant::operator-=(const var &x)               { return *this -= *x; }

types_extrapolate_real_p  (variant& variant::operator*=, { terror(real); rval.d *= x; return *this; })
types_extrapolate_string_p(variant& variant::operator*=, { terrortrue(); return *this; })
variant& variant::operator*=(const variant x)            { terror2(real); rval.d *= x.rval.d; return *this; }
variant& variant::operator*=(const var &x)               { return *this *= *x; }

types_extrapolate_real_p  (variant& variant::operator/=, { terror(real); rval.d /= x; return *this; })
types_extrapolate_string_p(variant& variant::operator/=, { terrortrue(); return *this; })
variant& variant::operator/=(const variant x)            { terror2(real); rval.d /= x.rval.d; return *this; }
variant& variant::operator/=(const var &x)               { return *this /= *x; }

types_extrapolate_real_p  (variant& variant::operator%=, { terror(real); rval.d = fmod(rval.d, x); return *this; })
types_extrapolate_string_p(variant& variant::operator%=, { terrortrue(); return *this; })
variant& variant::operator%=(const variant x)            { terror2(real); rval.d = fmod(rval.d, x.rval.d); return *this; }
variant& variant::operator%=(const var &x)               { div0c((*x).rval.d) rval.d = fmod(rval.d, (*x).rval.d); return *this; }


types_extrapolate_real_p  (variant& variant::operator<<=, { terror(real); rval.d = long(rval.d) << int(x); return *this; })
types_extrapolate_string_p(variant& variant::operator<<=, { terrortrue(); return *this; })
variant& variant::operator<<=(const variant x)            { terror2(real); rval.d = long(rval.d) << long(x.rval.d); return *this; }
variant& variant::operator<<=(const var &x)               { return *this <<= *x; }

types_extrapolate_real_p  (variant& variant::operator>>=, { terror(real); rval.d = long(rval.d) >> int(x); return *this; })
types_extrapolate_string_p(variant& variant::operator>>=, { terrortrue(); return *this; })
variant& variant::operator>>=(const variant x)            { terror2(real); rval.d = long(rval.d) >> long(x.rval.d); return *this; }
variant& variant::operator>>=(const var &x)               { return *this >>= *x; }

types_extrapolate_real_p  (variant& variant::operator&=,  { terror(real); rval.d = long(rval.d) & long(x); return *this; })
types_extrapolate_string_p(variant& variant::operator&=,  { terrortrue(); return *this; })
variant& variant::operator&=(const variant x)             { terror2(real); rval.d = long(rval.d) & long(x.rval.d); return *this; }
variant& variant::operator&=(const var &x)                { return *this &= *x; }

types_extrapolate_real_p  (variant& variant::operator|=,  { terror(real); rval.d = long(rval.d) | long(x); return *this; })
types_extrapolate_string_p(variant& variant::operator|=,  { terrortrue(); return *this; })
variant& variant::operator|=(const variant x)             { terror2(real); rval.d = long(rval.d) | long(x.rval.d); return *this; }
variant& variant::operator|=(const var &x)                { return *this |= *x; }

types_extrapolate_real_p  (variant& variant::operator^=,  { terror(real); rval.d = long(rval.d) ^ long(x); return *this; })
types_extrapolate_string_p(variant& variant::operator^=,  { terrortrue(); return *this; })
variant& variant::operator^=(const variant x)             { terror2(real); rval.d = long(rval.d) ^ long(x.rval.d); return *this; }
variant& variant::operator^=(const var &x)                { return *this ^= *x; }



#undef EVCONST
#define EVCONST const
types_extrapolate_real_p  (variant variant::operator+, { terror(real); return rval.d + x; })
types_extrapolate_string_p(variant variant::operator+, { terror(tstr); return sval   + x; })
variant variant::operator+(const variant x) EVCONST    { terror(x.type); if (x.type == real) return rval.d + x.rval.d; return sval + x.sval; }
variant variant::operator+(const var &x)    EVCONST    { return *this + *x; }

types_extrapolate_real_p  (double  variant::operator-, { terror(real); return rval.d - x; })
types_extrapolate_string_p(double  variant::operator-, { terrortrue(); return rval.d; })
double variant::operator-(const variant x)  EVCONST    { terror2(real); return rval.d - x.rval.d; }
double variant::operator-(const var &x)     EVCONST    { return *this - *x; }

types_extrapolate_real_p  (double  variant::operator*, { terror(real); return rval.d * x; })
types_extrapolate_string_p(double  variant::operator*, { terrortrue(); return rval.d; })
double variant::operator*(const variant x)  EVCONST    { terror2(real); return rval.d * x.rval.d; }
double variant::operator*(const var &x)     EVCONST    { return *this * *x; }

types_extrapolate_real_p  (double  variant::operator/, { terror(real); return rval.d / x; })
types_extrapolate_string_p(double  variant::operator/, { terrortrue(); return rval.d; })
double variant::operator/(const variant x)  EVCONST    { terror2(real); return rval.d / x.rval.d; }
double variant::operator/(const var &x)     EVCONST    { return *this / *x; }

types_extrapolate_real_p  (double  variant::operator%, { terror(real); return fmod(rval.d, x); })
types_extrapolate_string_p(double  variant::operator%, { terrortrue(); return rval.d; })
double variant::operator%(const variant x)  EVCONST    { terror2(real); div0c(x.rval.d); return fmod(rval.d, x.rval.d); }
double variant::operator%(const var &x)     EVCONST    { div0c((*x).rval.d); return fmod(this->rval.d, (*x).rval.d); }


types_extrapolate_real_p  (long variant::operator<<, { terror(real); return long(rval.d) << long(x); })
types_extrapolate_string_p(long variant::operator<<, { terrortrue(); return long(rval.d); })
long variant::operator<<(const variant x) EVCONST    { terror2(real); return long(rval.d) << long(x.rval.d); }
long variant::operator<<(const var &x)    EVCONST    { return *this << *x; }

types_extrapolate_real_p  (long variant::operator>>, { terror(real); return long(rval.d) >> long(x); })
types_extrapolate_string_p(long variant::operator>>, { terrortrue(); return long(rval.d); })
long variant::operator>>(const variant x) EVCONST    { terror2(real); return long(rval.d) >> long(x.rval.d); }
long variant::operator>>(const var &x)    EVCONST    { return *this >> *x; }

types_extrapolate_real_p  (long variant::operator&, { terror(real); return long(rval.d) & long(x); })
types_extrapolate_string_p(long variant::operator&, { terrortrue(); return long(rval.d); })
long variant::operator&(const variant x) EVCONST    { terror2(real); return long(rval.d) & long(x.rval.d); }
long variant::operator&(const var &x)    EVCONST    { return *this & *x; }

types_extrapolate_real_p  (long variant::operator|, { terror(real); return long(rval.d) | long(x); })
types_extrapolate_string_p(long variant::operator|, { terrortrue(); return long(rval.d);})
long variant::operator|(const variant x) EVCONST    { terror2(real); return long(rval.d) | long(x.rval.d); }
long variant::operator|(const var &x)    EVCONST    { return *this | *x; }

types_extrapolate_real_p  (long variant::operator^, { terror(real); return long(rval.d) ^ long(x); })
types_extrapolate_string_p(long variant::operator^, { terrortrue(); return long(rval.d); })
long variant::operator^(const variant x) EVCONST    { terror2(real); return long(rval.d) ^ long(x.rval.d); }
long variant::operator^(const var &x)    EVCONST    { return *this ^ *x; }


// STANDARD:  In all cases, string > real

types_extrapolate_real_p  (bool variant::operator==, { return type == real and vareq(rval.d, x); })
types_extrapolate_string_p(bool variant::operator==, { return type == tstr and sval == x; })
bool variant::operator==(const variant &x)   EVCONST { return type == x.type and ((x.type == real) ? vareq(rval.d, x.rval.d) : sval == x.sval); }
//bool variant::operator==(const variant x)            { return type == x.type and ((x.type == real) ? rval.d == x.rval.d : sval == x.sval); }
bool variant::operator==(const var &x)       EVCONST { return *this == *x; }

types_extrapolate_real_p  (bool variant::operator!=, { return type != real or varneq(rval.d, x); })
types_extrapolate_string_p(bool variant::operator!=, { return type != tstr or sval != x; })
bool variant::operator!=(const variant &x)   EVCONST { return type != x.type or ((x.type == real) ? varneq(rval.d, x.rval.d) : sval != x.sval); }
//bool variant::operator!=(const variant x)            { return type != x.type or ((x.type == real) ? rval.d != x.rval.d : sval != x.sval); }
bool variant::operator!=(const var &x)       EVCONST { return *this != *x; }

types_extrapolate_real_p  (bool variant::operator>=, { return type != real or  rval.d >= x - var_e; }) //type != real, then we're string and a priori greater.
types_extrapolate_string_p(bool variant::operator>=, { return type == tstr and sval   >= x; }) //To be more, we must be string anyway.
bool variant::operator>=(const variant &x)   EVCONST { return !(type < x.type) and (type > x.type or ((x.type == real) ? rval.d >= x.rval.d : sval >= x.sval)); }
//bool variant::operator>=(const variant x)            { return !(type < x.type) and (type > x.type or ((x.type == real) ? rval.d >= x.rval.d : sval >= x.sval)); }
bool variant::operator>=(const var &x)       EVCONST { return *this >= *x; }

types_extrapolate_real_p  (bool variant::operator<=, { return type == real and rval.d <= x + var_e; }) //To be less, we must be real anyway.
types_extrapolate_string_p(bool variant::operator<=, { return type != tstr or  sval   <= x; }) //type != tstr, then we're real and a priori less.
bool variant::operator<=(const variant &x)   EVCONST { return !(type > x.type) and (type < x.type or ((x.type == real) ? rval.d <= x.rval.d : sval <= x.sval)); }
//bool variant::operator<=(const variant x)            { return !(type > x.type) and (type < x.type or ((x.type == real) ? rval.d <= x.rval.d : sval <= x.sval)); }
bool variant::operator<=(const var &x)       EVCONST { return *this <= *x; }

types_extrapolate_real_p  (bool variant::operator>,  { return type != real or  rval.d > x + var_e; }) //type != real, then we're string and a priori greater.
types_extrapolate_string_p(bool variant::operator>,  { return type == tstr and sval   > x; }) //To be more, we must be string anyway.
bool variant::operator>(const variant &x)   EVCONST { return !(type < x.type) and (type > x.type or ((x.type == real) ? rval.d > x.rval.d : sval > x.sval)); }
//bool variant::operator>(const variant x)             { return !(type < x.type) and (type > x.type or ((x.type == real) ? rval.d > x.rval.d : sval > x.sval)); }
bool variant::operator>(const var &x)       EVCONST  { return *this > *x; }

types_extrapolate_real_p  (bool variant::operator<,  { return type == real and rval.d < x - var_e; }) //To be less, we must be real anyway.
types_extrapolate_string_p(bool variant::operator<,  { return type != tstr or  sval   < x; }) //type != tstr, then we're real and a priori less.
bool variant::operator<(const variant &x)   EVCONST { return !(type > x.type) and (type < x.type or ((x.type == real) ? rval.d < x.rval.d : sval < x.sval)); }
//bool variant::operator<(const variant x)             { return !(type > x.type) and (type < x.type or ((x.type == real) ? rval.d < x.rval.d : sval < x.sval)); }
bool variant::operator<(const var &x)       EVCONST  { return *this < *x; }

variant::~variant() { }

#undef EVCONST
#define EVCONST

/*
   This part is about ass-backwards. But we'll go along with it. For science. You monster.
*/
#ifdef DEBUG_MODE
  #undef div0c
  #undef terror
  #undef terror2
  #undef terrortrue
  
  #define div0c(x)
  #define terror(x)
  #define terror2(x)
  #define terrortrue()
#endif

types_binary_assign_extrapolate_implement(+, const variant&, terror(real);)
types_binary_assign_extrapolate_implement(-, const variant&, terror(real);)
types_binary_assign_extrapolate_implement(*, const variant&, terror(real);)
types_binary_assign_extrapolate_implement(/, const variant&, terror(real);)

types_binary_bitwise_assign_extrapolate_implement(<<, const variant&, terror(real);)
types_binary_bitwise_assign_extrapolate_implement(>>, const variant&, terror(real);)
types_binary_bitwise_assign_extrapolate_implement(&,  const variant&, terror(real);)
types_binary_bitwise_assign_extrapolate_implement(|,  const variant&, terror(real);)
types_binary_bitwise_assign_extrapolate_implement(^,  const variant&, terror(real);)

types_binary_extrapolate_real_p  (double operator+, const variant&, { terror(real); return x + y.rval.d; })
types_binary_extrapolate_string_p(string operator+, const variant&, { terror(tstr); return x + y.sval; })
types_binary_extrapolate_real_p  (double operator-, const variant&, { terror(real); return x - y.rval.d; })
types_binary_extrapolate_string_p(string operator-, const variant&, { terrortrue(); return 0; })
types_binary_extrapolate_real_p  (double operator*, const variant&, { terror(real); return x * y.rval.d; })
types_binary_extrapolate_string_p(string operator*, const variant&, { terrortrue(); return 0; })
types_binary_extrapolate_real_p  (double operator/, const variant&, { terror(real); div0c(y.rval.d); return x / y.rval.d; })
types_binary_extrapolate_string_p(string operator/, const variant&, { terrortrue(); return 0; })
types_binary_extrapolate_real_p  (double operator%, const variant&, { terror(real); div0c(y.rval.d); return fmod(x, y.rval.d); })
types_binary_extrapolate_string_p(string operator%, const variant&, { terrortrue(); return 0; })

types_binary_bitwise_extrapolate_real_p(<<, const variant&, terror(real);  )
//types_binary_extrapolate_string_p      (string operator<<, const variant&, { terrortrue(); return 0; })
types_binary_bitwise_extrapolate_real_p(>>, const variant&, terror(real);  )
//types_binary_extrapolate_string_p      (string operator>>, const variant&, { terrortrue(); return 0; })
types_binary_bitwise_extrapolate_real_p(&,  const variant&, terror(real);  )
//types_binary_extrapolate_string_p      (string operator&,  const variant&, { terrortrue(); return 0; })
types_binary_bitwise_extrapolate_real_p(|,  const variant&, terror(real);  )
//types_binary_extrapolate_string_p      (string operator|,  const variant&, { terrortrue(); return 0; })
types_binary_bitwise_extrapolate_real_p(^,  const variant&, terror(real);  )
//types_binary_extrapolate_string_p      (string operator^,  const variant&, { terrortrue(); return 0; })

// I have no fucking idea why C++0x can't do this for me.
types_binary_extrapolate_real_p  (bool operator==, const variant&, { return y == x; })
types_binary_extrapolate_string_p(bool operator==, const variant&, { return y == x; })
types_binary_extrapolate_real_p  (bool operator!=, const variant&, { return y != x; })
types_binary_extrapolate_string_p(bool operator!=, const variant&, { return y != x; })
types_binary_extrapolate_real_p  (bool operator>=, const variant&, { return y <= x; })
types_binary_extrapolate_string_p(bool operator>=, const variant&, { return y <= x; })
types_binary_extrapolate_real_p  (bool operator<=, const variant&, { return y >= x; })
types_binary_extrapolate_string_p(bool operator<=, const variant&, { return y >= x; })
types_binary_extrapolate_real_p  (bool operator>,  const variant&, { return y < x;  })
types_binary_extrapolate_string_p(bool operator>,  const variant&, { return y < x;  })
types_binary_extrapolate_real_p  (bool operator<,  const variant&, { return y > x;  })
types_binary_extrapolate_string_p(bool operator<,  const variant&, { return y > x;  })


/*
   Unary nonsense for either party
*/

char&     variant::operator[] (int x)    { return sval[x]; }
variant&  variant::operator++ ()         { return ++rval.d, *this; }
double    variant::operator++ (int)      { return rval.d++; }
variant&  variant::operator-- ()         { return --rval.d, *this; }
double    variant::operator-- (int)      { return rval.d--; }
variant&  variant::operator*  ()         { return *this; }

#undef EVCONST
#define EVCONST const
bool      variant::operator!  () EVCONST { return !bool(*this); }
long      variant::operator~  () EVCONST { return ~long(*this); }
double    variant::operator-  () EVCONST { return -rval.d; }
double    variant::operator+  () EVCONST { return  rval.d; }
#undef EVCONST
#define EVCONST

bool is_undefined(variant val)   { return val.type == ty_undefined; }
bool is_real(variant val)   { return val.type == real; }
bool is_string(variant val) { return val.type == tstr;  }
bool is_ptr(variant val)   { return val.type == ty_pointer; }

} //enigma_user