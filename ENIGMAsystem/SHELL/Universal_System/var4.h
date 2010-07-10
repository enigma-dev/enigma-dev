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

#ifndef _var4_h
#define _var4_h

#include <string>
using std::string;

// We want var and variant to support a lot of assignment types.

#include "var_te.h"

namespace enigma {
  union rvt {
    double d;
    void * p;
    rvt(double x): d(x) {}
  };
}

struct var;

struct variant
{
  enigma::rvt rval;
  string sval;
  int type;
  
  operator int();
  operator bool();
  operator char();
  operator long();
  operator short();
  operator unsigned();
  operator unsigned char();
  operator unsigned long();
  operator unsigned short();
  operator unsigned long long();
  operator long long();
  operator double();
  operator float();
  
  operator string();
  
  operator int() const;
  operator bool() const;
  operator char() const;
  operator long() const;
  operator short() const;
  operator unsigned() const;
  operator unsigned char() const;
  operator unsigned long() const;
  operator unsigned short() const;
  operator unsigned long long() const;
  operator long long() const;
  operator double() const;
  operator float() const;
  
  operator string() const;
  
  variant();
  types_extrapolate_alldecc(variant)
  
  types_extrapolate_alldec(variant& operator=)
  types_extrapolate_alldec(variant& operator+=)
  types_extrapolate_alldec(variant& operator-=)
  types_extrapolate_alldec(variant& operator*=)
  types_extrapolate_alldec(variant& operator/=)
  
  types_extrapolate_alldec(variant& operator<<=)
  types_extrapolate_alldec(variant& operator>>=)
  types_extrapolate_alldec(variant& operator&=)
  types_extrapolate_alldec(variant& operator|=)
  types_extrapolate_alldec(variant& operator^=)
  
  types_extrapolate_alldec(variant operator+)
  types_extrapolate_alldec(double  operator-)
  types_extrapolate_alldec(double  operator*)
  types_extrapolate_alldec(double  operator/)
  
  types_extrapolate_alldec(long operator<<)
  types_extrapolate_alldec(long operator>>)
  types_extrapolate_alldec(long operator&)
  types_extrapolate_alldec(long operator|)
  types_extrapolate_alldec(long operator^)
  
  #undef types_extrapolate_alldec
  #define types_extrapolate_alldec(prefix)\
   types_extrapolate_real_p  (prefix,;)\
   types_extrapolate_string_p(prefix,;)\
   prefix (const variant &x) const;\
   prefix (const variant x);\
   prefix (const var &x);
  
  types_extrapolate_alldec(bool operator==)
  types_extrapolate_alldec(bool operator!=)
  types_extrapolate_alldec(bool operator>=)
  types_extrapolate_alldec(bool operator<=)
  types_extrapolate_alldec(bool operator>)
  types_extrapolate_alldec(bool operator<)
  
  char&     operator[] (int);
  variant&  operator++();
  double    operator++(int);
  variant&  operator--();
  double    operator--(int);
  bool      operator!();
  variant&  operator*();
  double    operator-();
  double    operator+();
  
  ~variant();
};

#undef types_extrapolate_alldec
#define types_extrapolate_alldec(prefix)\
 types_extrapolate_real_p  (prefix,;)\
 types_extrapolate_string_p(prefix,;)\
 prefix (variant x);

struct var
{
  void *values;
  
  private:
    void initialize();
    void cleanup();
  public:
  
  operator variant&();
  operator const variant&() const;
  
  operator int();
  operator bool();
  operator char();
  operator long();
  operator short();
  operator unsigned();
  operator unsigned char();
  operator unsigned long();
  operator unsigned short();
  operator unsigned long long();
  operator long long();
  operator double();
  operator float();
  
  operator string();
  
  operator int() const;
  operator bool() const;
  operator char() const;
  operator long() const;
  operator short() const;
  operator unsigned() const;
  operator unsigned char() const;
  operator unsigned long() const;
  operator unsigned short() const;
  operator unsigned long long() const;
  operator long long() const;
  operator double() const;
  operator float() const;
  
  operator string() const;
  
  var();
  var(const var&);
  types_extrapolate_alldec(var);
  
  types_extrapolate_alldec(variant& operator=)
  var& operator= (const var&);
  
  #undef types_extrapolate_alldec
  #define types_extrapolate_alldec(prefix)\
   types_extrapolate_real_p  (prefix,;)\
   types_extrapolate_string_p(prefix,;)\
   prefix (const var& x);\
   prefix (variant x);
  
  types_extrapolate_alldec(variant& operator+=)
  types_extrapolate_alldec(variant& operator-=)
  types_extrapolate_alldec(variant& operator*=)
  types_extrapolate_alldec(variant& operator/=)
  
  types_extrapolate_alldec(variant& operator<<=)
  types_extrapolate_alldec(variant& operator>>=)
  types_extrapolate_alldec(variant& operator&=)
  types_extrapolate_alldec(variant& operator|=)
  types_extrapolate_alldec(variant& operator^=)
  
  types_extrapolate_alldec(variant operator+)
  types_extrapolate_alldec(double  operator-)
  types_extrapolate_alldec(double  operator*)
  types_extrapolate_alldec(double  operator/)
  
  types_extrapolate_alldec(long operator<<)
  types_extrapolate_alldec(long operator>>)
  types_extrapolate_alldec(long operator&)
  types_extrapolate_alldec(long operator|)
  types_extrapolate_alldec(long operator^)
  
  #undef types_extrapolate_alldec
  #define types_extrapolate_alldec(prefix)\
   types_extrapolate_real_p  (prefix,;)\
   types_extrapolate_string_p(prefix,;)\
   prefix (const variant &x) const;\
   prefix (const variant x);\
   prefix (const var& x) const;\
   prefix (const var x);
  
  types_extrapolate_alldec(bool operator==)
  types_extrapolate_alldec(bool operator!=)
  types_extrapolate_alldec(bool operator>=)
  types_extrapolate_alldec(bool operator<=)
  types_extrapolate_alldec(bool operator>)
  types_extrapolate_alldec(bool operator<)
  
  variant&  operator++();
  double    operator++(int);
  variant&  operator--();
  double    operator--(int);
  bool      operator!();
  double    operator-();
  double    operator+();
  
  variant& operator*  ();
  variant& operator[] (int);
  variant& operator() (int);
  variant& operator() (int,int);
  
  const variant& operator*  () const;
  const variant& operator[] (int) const;
  const variant& operator() (int) const;
  const variant& operator() (int,int) const;
  
  ~var();
};

string toString(const variant &a);
string toString(const var &a);


types_binary_assign_extrapolate_declare(+, variant)
types_binary_assign_extrapolate_declare(-, variant)
types_binary_assign_extrapolate_declare(*, variant)
types_binary_assign_extrapolate_declare(/, variant)

types_binary_assign_extrapolate_declare(<<, variant) 
types_binary_assign_extrapolate_declare(>>, variant) 
types_binary_assign_extrapolate_declare(&,  variant) 
types_binary_assign_extrapolate_declare(|,  variant) 
types_binary_assign_extrapolate_declare(^,  variant)


types_binary_extrapolate_alldecc(double, operator+, variant)
types_binary_extrapolate_alldecc(double, operator-, variant)
types_binary_extrapolate_alldecc(double, operator*, variant)
types_binary_extrapolate_alldecc(double, operator/, variant)

types_binary_bitwise_extrapolate_alldecc(operator<<, variant)
types_binary_bitwise_extrapolate_alldecc(operator>>, variant)
types_binary_bitwise_extrapolate_alldecc(operator&,  variant)
types_binary_bitwise_extrapolate_alldecc(operator|,  variant)
types_binary_bitwise_extrapolate_alldecc(operator^,  variant)

types_binary_extrapolate_alldecce(bool, operator==, variant)
types_binary_extrapolate_alldecce(bool, operator!=, variant)
types_binary_extrapolate_alldecce(bool, operator>=, variant)
types_binary_extrapolate_alldecce(bool, operator<=, variant)
types_binary_extrapolate_alldecce(bool, operator>,  variant)
types_binary_extrapolate_alldecce(bool, operator<,  variant)


/* Do the same for var
 *************************/

types_binary_assign_extrapolate_declare(+, var)
types_binary_assign_extrapolate_declare(-, var)
types_binary_assign_extrapolate_declare(*, var)
types_binary_assign_extrapolate_declare(/, var)

types_binary_assign_extrapolate_declare(<<, var) 
types_binary_assign_extrapolate_declare(>>, var) 
types_binary_assign_extrapolate_declare(&,  var) 
types_binary_assign_extrapolate_declare(|,  var) 
types_binary_assign_extrapolate_declare(^,  var)


types_binary_extrapolate_alldecc(double, operator+, var)
types_binary_extrapolate_alldecc(double, operator-, var)
types_binary_extrapolate_alldecc(double, operator*, var)
types_binary_extrapolate_alldecc(double, operator/, var)

types_binary_bitwise_extrapolate_alldecc(operator<<, var)
types_binary_bitwise_extrapolate_alldecc(operator>>, var)
types_binary_bitwise_extrapolate_alldecc(operator&,  var)
types_binary_bitwise_extrapolate_alldecc(operator|,  var)
types_binary_bitwise_extrapolate_alldecc(operator^,  var)

types_binary_extrapolate_alldecce(bool, operator==, var)
types_binary_extrapolate_alldecce(bool, operator!=, var)
types_binary_extrapolate_alldecce(bool, operator>=, var)
types_binary_extrapolate_alldecce(bool, operator<=, var)
types_binary_extrapolate_alldecce(bool, operator>,  var)
types_binary_extrapolate_alldecce(bool, operator<,  var)

#undef types_extrapolate_real_p
#undef types_extrapolate_string_p
#undef types_extrapolate_mix_p
#undef types_extrapolate_alldec
#undef types_extrapolate_alldecc

#define string(VALUE_TO_MAKE_A_STRING_OUT_OF) toString(VALUE_TO_MAKE_A_STRING_OUT_OF)

#undef unsigll

string toString(const variant &a);
string toString(const var &a);

#endif
