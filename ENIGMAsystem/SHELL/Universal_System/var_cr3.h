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

/*******************************************

DEVELOPER NOTICES

VAR is NOT A RETURN TYPE. VAR allows for multidimensional arrays, and is unsuitable
for returning a single string/real. IT WILL NOT FUNCTION AS A RETURN TYPE.

To return a string/real, use the correct C++ return type, or, if it must return
either, use enigma::variant. enigma::variant is lighter weight, and is designed
to be copied, not just passed as a reference.

*******************************************/

#ifndef _var_cr3_h
#define _var_cr3_h

#include <string>

#define REAL__ double //Use double as the real type
#define STRING__ std::string //Use std::string as the string type
#define FREEOLD__ true //Free strings when switching to real, zero real when switching to string

#if DEBUGMODE && SHOWERRORS
#define terr(t1,t2) if (t1 != t2) show_error("Performing operations on different types",0);
#define serr(t1,t2) if (t1 == 1 || t2 == 1) show_error("Performing invalid operations on strings",0); else if (t1 != t2) show_error("Performing operations on different types",0);
#define uerr(t)     if (t==1) show_error("Performing invalid unary operation on string",0);
#else
#define terr(t1,t2)
#define serr(t1,t2)
#define uerr(t)
#endif


#if FREEOLD__
#define clearstr() stringval="";
#define clearreal() realval=0;
#define varclearstr() values[0]->stringval="";
#define varclearreal() values[0]->realval=0;
#else
#define clearstr()
#define clearreal()
#define varclearstr()
#define varclearreal()
#endif


struct var;

namespace enigma
{
  struct variant
  {
    REAL__   realval;
    STRING__ stringval;
    bool type;

    variant(void);

    variant(int x);
    variant(double x);
    variant(const char* x);
    variant(std::string x);

    virtual ~variant();

    virtual char &operator[] (int x);

    const char* c_str() ;


    operator double&();
    operator std::string&();

    //These are assignment operators and require a reference to be passed
    virtual double &operator= (int y),  &operator= (double y);   virtual std::string &operator= (std::string y),  &operator= (char* y);   virtual enigma::variant& operator= (enigma::variant y), &operator= (var& y);
    virtual double &operator+= (int y), &operator+= (double y);  virtual std::string &operator+= (std::string y), &operator+= (char* y);  virtual enigma::variant &operator+= (enigma::variant y), &operator+= (var& y);
    virtual double &operator-= (int y), &operator-= (double y);  virtual std::string &operator-= (std::string y), &operator-= (char* y);  virtual enigma::variant &operator-= (enigma::variant y), &operator-= (var& y);
    virtual double &operator*= (int y), &operator*= (double y);  virtual std::string &operator*= (std::string y), &operator*= (char* y);  virtual enigma::variant &operator*= (enigma::variant y), &operator*= (var& y);
    virtual double &operator/= (int y), &operator/= (double y);  virtual std::string &operator/= (std::string y), &operator/= (char* y);  virtual enigma::variant &operator/= (enigma::variant y), &operator/= (var& y);
    virtual double &operator<<= (int y),&operator<<= (double y); virtual std::string &operator<<= (std::string y),&operator<<= (char* y); virtual enigma::variant &operator<<= (enigma::variant y),&operator<<= (var& y);
    virtual double &operator>>= (int y),&operator>>= (double y); virtual std::string &operator>>= (std::string y),&operator>>= (char* y); virtual enigma::variant &operator>>= (enigma::variant y),&operator>>= (var& y);

    virtual double &operator&= (int y), &operator&= (double y);  virtual std::string &operator&= (std::string y), &operator&= (char* y);  virtual enigma::variant &operator&= (enigma::variant y), &operator&= (var& y);
    virtual double &operator|= (int y), &operator|= (double y);  virtual std::string &operator|= (std::string y), &operator|= (char* y);  virtual enigma::variant &operator|= (enigma::variant y), &operator|= (var& y);
    virtual double &operator^= (int y), &operator^= (double y);  virtual std::string &operator^= (std::string y), &operator^= (char* y);  virtual enigma::variant &operator^= (enigma::variant y), &operator^= (var& y);

    //These are mathematical/concatentation operators and should always return double, except for +, which can return either type.
    double operator+ (int y), operator+ (double y); std::string operator+ (std::string y),operator+ (char* y); enigma::variant operator+ (enigma::variant y), operator+ (var& y);
    double operator- (int y), operator- (double y), operator- (std::string y), operator- (char* y),  operator- (enigma::variant y),  operator- (var& y);
    double operator* (int y), operator* (double y), operator* (std::string y), operator* (char* y),  operator* (enigma::variant y),  operator* (var& y);
    double operator/ (int y), operator/ (double y), operator/ (std::string y), operator/ (char* y),  operator/ (enigma::variant y),  operator/ (var& y);
    double operator% (int y), operator% (double y), operator% (std::string y), operator% (char* y),  operator% (enigma::variant y),  operator% (var& y);
    double operator<< (int y),operator<< (double y),operator<< (std::string y),operator<< (char* y), operator<< (enigma::variant y), operator<< (var& y);
    double operator>> (int y),operator>> (double y),operator>> (std::string y),operator>> (char* y), operator>> (enigma::variant y), operator>> (var& y);

    double operator& (int y), operator& (double y), operator& (std::string y), operator& (char* y),  operator& (enigma::variant y),  operator& (var& y);
    double operator| (int y), operator| (double y), operator| (std::string y), operator| (char* y),  operator| (enigma::variant y),  operator| (var& y);
    double operator^ (int y), operator^ (double y), operator^ (std::string y), operator^ (char* y),  operator^ (enigma::variant y),  operator^ (var& y);

    //Unary
    bool operator!();
    double operator~();
    double operator-();
    double operator+();
    enigma::variant* operator*();

    //Comparison
    bool operator< (int y),  operator< (double y),  operator< (std::string y),  operator< (char* y),  operator< (enigma::variant y),  operator< (var& y);
    bool operator> (int y),  operator> (double y),  operator> (std::string y),  operator> (char* y),  operator> (enigma::variant y),  operator> (var& y);
    bool operator<= (int y), operator<= (double y), operator<= (std::string y), operator<= (char* y), operator<= (enigma::variant y), operator<= (var& y);
    bool operator>= (int y), operator>= (double y), operator>= (std::string y), operator>= (char* y), operator>= (enigma::variant y), operator>= (var& y);
    bool operator== (int y), operator== (double y), operator== (std::string y), operator== (char* y), operator== (enigma::variant y), operator== (var& y);
    bool operator!= (int y), operator!= (double y), operator!= (std::string y), operator!= (char* y), operator!= (enigma::variant y), operator!= (var& y);
  };
}

struct var
{
  enigma::variant** values;
  int x,y,xsize,ysize;
  bool initd;

  void resize(int xn,int yn);

  enigma::variant& operator[] (double x);
  enigma::variant& operator() (double x);
  enigma::variant& operator() (double x,double y);

  char* c_str();

  var(void);

  var(int v);
  var(double v);
  var(const char* v);
  var(std::string v);
  var(const var &v);

  ~var (void);

  //These are for operator= with other things
  operator enigma::variant&();
  operator double&();
  operator std::string&();



  //These are assignment operators and require a reference to be passed
  double &operator= (int y),  &operator= (double y);   std::string &operator= (std::string y),  &operator= (char* y);   enigma::variant &operator= (enigma::variant y),  &operator= (var& y);
  double &operator+= (int y), &operator+= (double y);  std::string &operator+= (std::string y), &operator+= (char* y);  enigma::variant &operator+= (enigma::variant y), &operator+= (var& y);
  double &operator-= (int y), &operator-= (double y);  std::string &operator-= (std::string y), &operator-= (char* y);  enigma::variant &operator-= (enigma::variant y), &operator-= (var& y);
  double &operator*= (int y), &operator*= (double y);  std::string &operator*= (std::string y), &operator*= (char* y);  enigma::variant &operator*= (enigma::variant y), &operator*= (var& y);
  double &operator/= (int y), &operator/= (double y);  std::string &operator/= (std::string y), &operator/= (char* y);  enigma::variant &operator/= (enigma::variant y), &operator/= (var& y);
  double &operator<<= (int y),&operator<<= (double y); std::string &operator<<= (std::string y),&operator<<= (char* y); enigma::variant &operator<<= (enigma::variant y),&operator<<= (var& y);
  double &operator>>= (int y),&operator>>= (double y); std::string &operator>>= (std::string y),&operator>>= (char* y); enigma::variant &operator>>= (enigma::variant y),&operator>>= (var& y);

  double &operator&= (int y), &operator&= (double y);  std::string &operator&= (std::string y), &operator&= (char* y);  enigma::variant &operator&= (enigma::variant y), &operator&= (var& y);
  double &operator|= (int y), &operator|= (double y);  std::string &operator|= (std::string y), &operator|= (char* y);  enigma::variant &operator|= (enigma::variant y), &operator|= (var& y);
  double &operator^= (int y), &operator^= (double y);  std::string &operator^= (std::string y), &operator^= (char* y);  enigma::variant &operator^= (enigma::variant y), &operator^= (var& y);

  //These are mathematical/concatentation operators and should always return double, except for +, which can return either type.
  double operator+ (int y), operator+ (double y); std::string operator+ (std::string y),operator+ (char* y); enigma::variant operator+ (enigma::variant y), operator+ (var& y);
  double operator- (int y), operator- (double y), operator- (std::string y), operator- (char* y),  operator- (enigma::variant y),  operator- (var& y);
  double operator* (int y), operator* (double y), operator* (std::string y), operator* (char* y),  operator* (enigma::variant y),  operator* (var& y);
  double operator/ (int y), operator/ (double y), operator/ (std::string y), operator/ (char* y),  operator/ (enigma::variant y),  operator/ (var& y);
  double operator% (int y), operator% (double y), operator% (std::string y), operator% (char* y),  operator% (enigma::variant y),  operator% (var& y);
  double operator<< (int y),operator<< (double y),operator<< (std::string y),operator<< (char* y), operator<< (enigma::variant y), operator<< (var& y);
  double operator>> (int y),operator>> (double y),operator>> (std::string y),operator>> (char* y), operator>> (enigma::variant y), operator>> (var& y);

  double operator& (int y), operator& (double y), operator& (std::string y), operator& (char* y),  operator& (enigma::variant y),  operator& (var& y);
  double operator| (int y), operator| (double y), operator| (std::string y), operator| (char* y),  operator| (enigma::variant y),  operator| (var& y);
  double operator^ (int y), operator^ (double y), operator^ (std::string y), operator^ (char* y),  operator^ (enigma::variant y),  operator^ (var& y);

  //Unary
  bool operator!();
  double operator~();
  double operator-();
  double operator+();
  enigma::variant** operator*();

  //Comparison
  bool operator< (int y),  operator< (double y),  operator< (std::string y),  operator< (char* y),  operator< (enigma::variant y),  operator< (var& y);
  bool operator> (int y),  operator> (double y),  operator> (std::string y),  operator> (char* y),  operator> (enigma::variant y),  operator> (var& y);
  bool operator<= (int y), operator<= (double y), operator<= (std::string y), operator<= (char* y), operator<= (enigma::variant y), operator<= (var& y);
  bool operator>= (int y), operator>= (double y), operator>= (std::string y), operator>= (char* y), operator>= (enigma::variant y), operator>= (var& y);
  bool operator== (int y), operator== (double y), operator== (std::string y), operator== (char* y), operator== (enigma::variant y), operator== (var& y);
  bool operator!= (int y), operator!= (double y), operator!= (std::string y), operator!= (char* y), operator!= (enigma::variant y), operator!= (var& y);
};

#endif //defined _var_cr3_h
