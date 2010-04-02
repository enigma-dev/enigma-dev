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

#include <string>
#include "var_cr3.h"


#define REAL__ double //Use double as the real type
#define STRING__ std::string //Use std::string as the string type
#define FREEOLD__ true //Free strings when switching to real, zero real when switching to string

#if DEBUGMODE && SHOWERRORS
#define terr(t1,t2) if (t1 != t2) show_error("Performing operations on different types",0);
#define serr(t1,t2) if (t1 == 1 || t2 == 1) show_error("Performing invalid operations on string",0); else if (t1 != t2) show_error("Performing operations on different types",0);
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


namespace enigma{
    variant::variant(void)          { realval=0; stringval=""; type=0; }
    variant::~variant()     {                                  }

    variant::variant(int x)         { realval=x; stringval=""; type=0; }
    variant::variant(double x)      { realval=x; stringval=""; type=0; }
    variant::variant(const char* x) { realval=0; stringval=x;  type=1; }
    variant::variant(std::string x) { realval=0; stringval=x;  type=1; }

    char &variant::operator[] (int x) { return stringval[x]; }

    const char* variant::c_str()      { return stringval.c_str(); }


    variant::operator double&()           { return realval;   }
    variant::operator std::string&()      { return stringval; }
}

void var::resize(int xn,int yn){
    if (xn<xsize && yn<ysize) return;

    xn++; if (xn<1)xn=1;
    yn++; if (yn<1)xn=1;

    int xo=xsize,yo=ysize;
    if (xn>xsize) xsize=xn; else xn=xsize;
    if (yn>ysize) ysize=yn; else yn=ysize;

    enigma::variant** oldvalues=initd?values:0;

    values=new enigma::variant*[xn*yn];

    if (initd)
    {
      int ix;
      for (ix=0;ix<xo;ix++)
      {
        int iy;

        for (iy=0;iy<yo;iy++)
        values[ix*yn+iy]=oldvalues[ix*yo+iy];

        for (;iy<yn;iy++)
        values[ix*yn+iy]=new enigma::variant;
      }
      for (ix=ix;ix<xn;ix++)
      {
        for (int iy=0;iy<yn;iy++)
        values[ix*yn+iy]=new enigma::variant;
      }

      delete[] oldvalues;
    }
    else
    {
      for (int ixy=0;ixy<xn*yn;ixy++)
      {
        values[ixy]=new enigma::variant;
      }
    }


    initd=1;
}

enigma::variant& var::operator[] (double x)          { resize((int)x,0);      return *values[(int)x*ysize]; }
enigma::variant& var::operator() (double x)          { resize((int)x,0);      return *values[(int)x*ysize]; }
enigma::variant& var::operator() (double x,double y) { resize((int)x,(int)y); return *values[(int)x*ysize+(int)y]; }

char* var::c_str()  { return (char*) values[0]->stringval.c_str(); }

var::var(void)           { x=0; y=0; xsize=0; ysize=0; initd=0; resize(0,0); }

var::var(int v)          { x=0; y=0; xsize=0; ysize=0; initd=0; resize(0,0); values[0]->realval=v; values[0]->type=0; }
var::var(double v)       { x=0; y=0; xsize=0; ysize=0; initd=0; resize(0,0); values[0]->realval=v; values[0]->type=0; }
var::var(const char* v)  { x=0; y=0; xsize=0; ysize=0; initd=0; resize(0,0); values[0]->stringval=v; values[0]->type=1; }
var::var(std::string v)  { x=0; y=0; xsize=0; ysize=0; initd=0; resize(0,0); values[0]->stringval=v; values[0]->type=1; }
var::var(const var &v)   { x=0; y=0; xsize=0; ysize=0; initd=0; resize(0,0); values[0]->stringval=v.values[0]->stringval; values[0]->realval=v.values[0]->realval; values[0]->type=v.values[0]->type; }

var::~var (void)
{
   for (int i=0;i<xsize*ysize;i++) delete values[i];
   delete []values;
}

//These are for operator= with other things
var::operator enigma::variant&()  { return *values[0];           }
var::operator double&()           { return values[0]->realval;   }
var::operator std::string&()      { return values[0]->stringval; }


/*********************************
 Assignment operators for VARIANT
*********************************/

double          &enigma::variant::operator= (int y)                { realval=y; clearstr(); type=0; return realval; }
double          &enigma::variant::operator= (double y)             { realval=y; clearstr(); type=0; return realval; }
std::string     &enigma::variant::operator= (std::string y)        { stringval=y; clearreal(); type=1; return stringval; }
std::string     &enigma::variant::operator= (char* y)              { stringval=y; clearreal(); type=1; return stringval; }
enigma::variant &enigma::variant::operator= (enigma::variant y)    { realval=y.realval; stringval=y.stringval; type=y.type; return *this; }
enigma::variant &enigma::variant::operator= (var& y)               { realval=y.values[0]->realval; stringval=y.values[0]->stringval; type=y.values[0]->type;; return *this; }

double          &enigma::variant::operator+= (int y)               { terr(0,type); realval+=y; return realval; }
double          &enigma::variant::operator+= (double y)            { terr(0,type); realval+=y; return realval; }
std::string     &enigma::variant::operator+= (std::string y)       { terr(1,type); stringval+=y; return stringval; }
std::string     &enigma::variant::operator+= (char* y)             { terr(1,type); stringval+=y; return stringval; }
enigma::variant &enigma::variant::operator+= (enigma::variant y)   { terr(y.type,type); if (y.type==0) realval+=y.realval; else stringval+=y.stringval; return *this; }
enigma::variant &enigma::variant::operator+= (var& y)              { terr(y.values[0]->type,type); if (y.values[0]->type==0) realval+=y.values[0]->realval; else stringval+=y.values[0]->stringval; return *this; }

double          &enigma::variant::operator-= (int y)               { terr(0,type); realval-=y; return realval; }
double          &enigma::variant::operator-= (double y)            { terr(0,type); realval-=y; return realval; }
std::string     &enigma::variant::operator-= (std::string y)       { terr(0,1); return stringval; }
std::string     &enigma::variant::operator-= (char* y)             { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator-= (enigma::variant y)   { terr(y.type,type); realval-=y.realval; return *this; }
enigma::variant &enigma::variant::operator-= (var& y)              { terr(y.values[0]->type,type); realval-=y.values[0]->realval; return *this; }

double          &enigma::variant::operator*= (int y)               { terr(0,type); realval*=y; return realval; }
double          &enigma::variant::operator*= (double y)            { terr(0,type); realval*=y; return realval; }
std::string     &enigma::variant::operator*= (std::string y)       { terr(0,1); return stringval; }
std::string     &enigma::variant::operator*= (char* y)             { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator*= (enigma::variant y)   { terr(y.type,type); realval*=y.realval; return *this; }
enigma::variant &enigma::variant::operator*= (var& y)              { terr(y.values[0]->type,type); realval*=y.values[0]->realval; return *this; }

double          &enigma::variant::operator/= (int y)               { terr(0,type); realval/=y; return realval; }
double          &enigma::variant::operator/= (double y)            { terr(0,type); realval/=y; return realval; }
std::string     &enigma::variant::operator/= (std::string y)       { terr(0,1); return stringval; }
std::string     &enigma::variant::operator/= (char* y)             { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator/= (enigma::variant y)   { terr(y.type,type); realval/=y.realval; return *this; }
enigma::variant &enigma::variant::operator/= (var& y)              { terr(y.values[0]->type,type); realval/=y.values[0]->realval; return *this; }

double          &enigma::variant::operator<<= (int y)              { terr(0,type); realval=(int)realval<<y; return realval; }
double          &enigma::variant::operator<<= (double y)           { terr(0,type); realval=int(realval+.5)<<(int)y; return realval; }
std::string     &enigma::variant::operator<<= (std::string y)      { terr(0,1); return stringval; }
std::string     &enigma::variant::operator<<= (char* y)            { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator<<= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)<<(int)y.realval; return *this; }
enigma::variant &enigma::variant::operator<<= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)<<(int)y.values[0]->realval; return *this; }

double          &enigma::variant::operator>>= (int y)              { terr(0,type); realval=int(realval+.5)>>y; return realval; }
double          &enigma::variant::operator>>= (double y)           { terr(0,type); realval=int(realval+.5)>>(int)y; return realval; }
std::string     &enigma::variant::operator>>= (std::string y)      { terr(0,1); return stringval; }
std::string     &enigma::variant::operator>>= (char* y)            { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator>>= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)>>(int)y.realval; return *this; }
enigma::variant &enigma::variant::operator>>= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)>>(int)y.values[0]->realval; return *this; }







double          &enigma::variant::operator&= (int y)              { terr(0,type); realval=(int)realval&y; return realval; }
double          &enigma::variant::operator&= (double y)           { terr(0,type); realval=int(realval+.5)&(int)y; return realval; }
std::string     &enigma::variant::operator&= (std::string y)      { terr(0,1); return stringval; }
std::string     &enigma::variant::operator&= (char* y)            { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator&= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)&(int)y.realval; return *this; }
enigma::variant &enigma::variant::operator&= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)&(int)y.values[0]->realval; return *this; }

double          &enigma::variant::operator|= (int y)              { terr(0,type); realval=(int)realval|y; return realval; }
double          &enigma::variant::operator|= (double y)           { terr(0,type); realval=int(realval+.5)|(int)y; return realval; }
std::string     &enigma::variant::operator|= (std::string y)      { terr(0,1); return stringval; }
std::string     &enigma::variant::operator|= (char* y)            { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator|= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)|(int)y.realval; return *this; }
enigma::variant &enigma::variant::operator|= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)|(int)y.values[0]->realval; return *this; }

double          &enigma::variant::operator^= (int y)              { terr(0,type); realval=(int)realval^y; return realval; }
double          &enigma::variant::operator^= (double y)           { terr(0,type); realval=int(realval+.5)^(int)y; return realval; }
std::string     &enigma::variant::operator^= (std::string y)      { terr(0,1); return stringval; }
std::string     &enigma::variant::operator^= (char* y)            { terr(0,1); return stringval; }
enigma::variant &enigma::variant::operator^= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)^(int)y.realval; return *this; }
enigma::variant &enigma::variant::operator^= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)^(int)y.values[0]->realval; return *this; }




/*****************************
 Assignment operators for VAR
*****************************/

double          &var::operator= (int y)                            { values[0]->realval=y; varclearstr(); values[0]->type=0; return values[0]->realval; }
double          &var::operator= (double y)                         { values[0]->realval=y; varclearstr(); values[0]->type=0; return values[0]->realval; }
std::string     &var::operator= (std::string y)                    { values[0]->stringval=y; varclearreal(); values[0]->type=1; return values[0]->stringval; }
std::string     &var::operator= (char* y)                          { values[0]->stringval=y; varclearreal(); values[0]->type=1; return values[0]->stringval; }
enigma::variant &var::operator= (enigma::variant y)                { values[0]->realval=y.realval; values[0]->stringval=y.stringval; values[0]->type=y.type; return *values[0]; }
enigma::variant &var::operator= (var& y)                           { values[0]->realval=y.values[0]->realval; values[0]->stringval=y.values[0]->stringval; values[0]->type=y.values[0]->type;; return *values[0]; }

double          &var::operator+= (int y)                           { terr(0,values[0]->type); values[0]->realval+=y; return values[0]->realval; }
double          &var::operator+= (double y)                        { terr(0,values[0]->type); values[0]->realval+=y; return values[0]->realval; }
std::string     &var::operator+= (std::string y)                   { terr(1,values[0]->type); values[0]->stringval+=y; return values[0]->stringval; }
std::string     &var::operator+= (char* y)                         { terr(1,values[0]->type); values[0]->stringval+=y; return values[0]->stringval; }
enigma::variant &var::operator+= (enigma::variant y)               { terr(y.type,values[0]->type); if (y.type==0) values[0]->realval+=y.realval; else values[0]->stringval+=y.stringval; return *values[0]; }
enigma::variant &var::operator+= (var& y)                          { terr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) values[0]->realval+=y.values[0]->realval; else values[0]->stringval+=y.values[0]->stringval; return *values[0]; }

double          &var::operator-= (int y)                           { terr(0,values[0]->type); values[0]->realval-=y; return values[0]->realval; }
double          &var::operator-= (double y)                        { terr(0,values[0]->type); values[0]->realval-=y; return values[0]->realval; }
std::string     &var::operator-= (std::string y)                   { terr(0,1); return values[0]->stringval; }
std::string     &var::operator-= (char* y)                         { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator-= (enigma::variant y)               { terr(y.type,values[0]->type); values[0]->realval-=y.realval; return *values[0]; }
enigma::variant &var::operator-= (var& y)                          { terr(y.values[0]->type,values[0]->type); values[0]->realval-=y.values[0]->realval; return *values[0]; }

double          &var::operator*= (int y)                           { terr(0,values[0]->type); values[0]->realval*=y; return values[0]->realval; }
double          &var::operator*= (double y)                        { terr(0,values[0]->type); values[0]->realval*=y; return values[0]->realval; }
std::string     &var::operator*= (std::string y)                   { terr(0,1); return values[0]->stringval; }
std::string     &var::operator*= (char* y)                         { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator*= (enigma::variant y)               { terr(y.type,values[0]->type); values[0]->realval*=y.realval; return *values[0]; }
enigma::variant &var::operator*= (var& y)                          { terr(y.values[0]->type,values[0]->type); values[0]->realval*=y.values[0]->realval; return *values[0]; }

double          &var::operator/= (int y)                           { terr(0,values[0]->type); values[0]->realval/=y; return values[0]->realval; }
double          &var::operator/= (double y)                        { terr(0,values[0]->type); values[0]->realval/=y; return values[0]->realval; }
std::string     &var::operator/= (std::string y)                   { terr(0,1); return values[0]->stringval; }
std::string     &var::operator/= (char* y)                         { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator/= (enigma::variant y)               { terr(y.type,values[0]->type); values[0]->realval/=y.realval; return *values[0]; }
enigma::variant &var::operator/= (var& y)                          { terr(y.values[0]->type,values[0]->type); values[0]->realval/=y.values[0]->realval; return *values[0]; }

double          &var::operator<<= (int y)                          { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)<<y; return values[0]->realval; }
double          &var::operator<<= (double y)                       { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)<<(int)y; return values[0]->realval; }
std::string     &var::operator<<= (std::string y)                  { terr(0,1); return values[0]->stringval; }
std::string     &var::operator<<= (char* y)                        { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator<<= (enigma::variant y)              { terr(y.type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)<<(int)y.realval; return *values[0]; }
enigma::variant &var::operator<<= (var& y)                         { terr(y.values[0]->type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)<<(int)y.values[0]->realval; return *values[0]; }

double          &var::operator>>= (int y)                          { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)>>y; return values[0]->realval; }
double          &var::operator>>= (double y)                       { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)>>(int)y; return values[0]->realval; }
std::string     &var::operator>>= (std::string y)                  { terr(0,1); return values[0]->stringval; }
std::string     &var::operator>>= (char* y)                        { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator>>= (enigma::variant y)              { terr(y.type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)>>(int)y.realval; return *values[0]; }
enigma::variant &var::operator>>= (var& y)                         { terr(y.values[0]->type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)>>(int)y.values[0]->realval; return *values[0]; }







double          &var::operator&= (int y)                          { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)&y; return values[0]->realval; }
double          &var::operator&= (double y)                       { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)&(int)y; return values[0]->realval; }
std::string     &var::operator&= (std::string y)                  { terr(0,1); return values[0]->stringval; }
std::string     &var::operator&= (char* y)                        { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator&= (enigma::variant y)              { terr(y.type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)&(int)y.realval; return *values[0]; }
enigma::variant &var::operator&= (var& y)                         { terr(y.values[0]->type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)&(int)y.values[0]->realval; return *values[0]; }

double          &var::operator|= (int y)                          { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)|y; return values[0]->realval; }
double          &var::operator|= (double y)                       { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)|(int)y; return values[0]->realval; }
std::string     &var::operator|= (std::string y)                  { terr(0,1); return values[0]->stringval; }
std::string     &var::operator|= (char* y)                        { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator|= (enigma::variant y)              { terr(y.type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)|(int)y.realval; return *values[0]; }
enigma::variant &var::operator|= (var& y)                         { terr(y.values[0]->type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)|(int)y.values[0]->realval; return *values[0]; }

double          &var::operator^= (int y)                          { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)^y; return values[0]->realval; }
double          &var::operator^= (double y)                       { terr(0,values[0]->type); values[0]->realval=int(values[0]->realval+.5)^(int)y; return values[0]->realval; }
std::string     &var::operator^= (std::string y)                  { terr(0,1); return values[0]->stringval; }
std::string     &var::operator^= (char* y)                        { terr(0,1); return values[0]->stringval; }
enigma::variant &var::operator^= (enigma::variant y)              { terr(y.type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)^(int)y.realval; return *values[0]; }
enigma::variant &var::operator^= (var& y)                         { terr(y.values[0]->type,values[0]->type); values[0]->realval=int(values[0]->realval+.5)^(int)y.values[0]->realval; return *values[0]; }





/*******************************
End assignment operator section
*******************************/













/***********************************
 Mathematical operators for VARIANT
***********************************/

double          enigma::variant::operator+ (int y)                { terr(0,type); return realval+y; }
double          enigma::variant::operator+ (double y)             { terr(0,type); return realval+y; }
std::string     enigma::variant::operator+ (std::string y)        { terr(1,type); return stringval+y; }
std::string     enigma::variant::operator+ (char* y)              { terr(1,type); return stringval+y; }
enigma::variant enigma::variant::operator+ (enigma::variant y)    { terr(y.type,type); if (y.type==0) return realval+y.realval; return stringval+y.stringval; }
enigma::variant enigma::variant::operator+ (var& y)               { terr(y.values[0]->type,type); if (y.values[0]->type==0) return realval+y.values[0]->realval; return stringval+y.values[0]->stringval; }

double          enigma::variant::operator- (int y)                { serr(0,type); return realval-y; }
double          enigma::variant::operator- (double y)             { serr(0,type); return realval-y; }
double          enigma::variant::operator- (std::string y)        { serr(1,0); return realval; }
double          enigma::variant::operator- (char* y)              { serr(1,0); return realval; }
double          enigma::variant::operator- (enigma::variant y)    { serr(y.type,type); if (y.type==0) return realval-y.realval; return realval; }
double          enigma::variant::operator- (var& y)               { serr(y.values[0]->type,type); if (y.values[0]->type==0) return realval-y.values[0]->realval; return realval; }

double          enigma::variant::operator* (int y)                { serr(0,type); return realval*y; }
double          enigma::variant::operator* (double y)             { serr(0,type); return realval*y; }
double          enigma::variant::operator* (std::string y)        { serr(1,0); return realval; }
double          enigma::variant::operator* (char* y)              { serr(1,0); return realval; }
double          enigma::variant::operator* (enigma::variant y)    { serr(y.type,type); if (y.type==0) return realval*y.realval; return realval; }
double          enigma::variant::operator* (var& y)               { serr(y.values[0]->type,type); if (y.values[0]->type==0) return realval*y.values[0]->realval; return realval; }

double          enigma::variant::operator/ (int y)                { serr(0,type); return realval/y; }
double          enigma::variant::operator/ (double y)             { serr(0,type); return realval/y; }
double          enigma::variant::operator/ (std::string y)        { serr(1,0); return realval; }
double          enigma::variant::operator/ (char* y)              { serr(1,0); return realval; }
double          enigma::variant::operator/ (enigma::variant y)    { serr(y.type,type); if (y.type==0) return realval/y.realval; return realval; }
double          enigma::variant::operator/ (var& y)               { serr(y.values[0]->type,type); if (y.values[0]->type==0) return realval/y.values[0]->realval; return realval; }

double          enigma::variant::operator% (int y)                { serr(0,type); double a=realval/y; a-=(int)a; return a*y; }
double          enigma::variant::operator% (double y)             { serr(0,type); double a=realval/y; a-=(int)a; return a*y; }
double          enigma::variant::operator% (std::string y)        { serr(1,0); return realval; }
double          enigma::variant::operator% (char* y)              { serr(1,0); return realval; }
double          enigma::variant::operator% (enigma::variant y)    { serr(y.type,type); if (y.type==0) { double a=realval/y.realval; a-=(int)a; return a*y.realval; } return realval; }
double          enigma::variant::operator% (var& y)               { serr(y.values[0]->type,type); if (y.values[0]->type==0) { double a=realval/y.values[0]->realval; a-=(int)a; return a*y.values[0]->realval; } return realval; }

double          enigma::variant::operator<< (int y)               { serr(0,type); return int(realval+.5)<<int(y); }
double          enigma::variant::operator<< (double y)            { serr(0,type); return int(realval+.5)<<int(y); }
double          enigma::variant::operator<< (std::string y)       { serr(1,0); return realval; }
double          enigma::variant::operator<< (char* y)             { serr(1,0); return realval; }
double          enigma::variant::operator<< (enigma::variant y)   { serr(y.type,type); if (y.type==0) return int(realval+.5)<<int(y.realval); return realval; }
double          enigma::variant::operator<< (var& y)              { serr(y.values[0]->type,type); if (y.values[0]->type==0) return int(realval+.5)<<int(y.values[0]->realval); return realval; }

double          enigma::variant::operator>> (int y)               { serr(0,type); return int(realval+.5)>>int(y); }
double          enigma::variant::operator>> (double y)            { serr(0,type); return int(realval+.5)>>int(y); }
double          enigma::variant::operator>> (std::string y)       { serr(1,0); return realval; }
double          enigma::variant::operator>> (char* y)             { serr(1,0); return realval; }
double          enigma::variant::operator>> (enigma::variant y)   { serr(y.type,type); if (y.type==0) return int(realval+.5)>>int(y.realval); return realval; }
double          enigma::variant::operator>> (var& y)              { serr(y.values[0]->type,type); if (y.values[0]->type==0) return int(realval+.5)>>int(y.values[0]->realval); return realval; }







double          enigma::variant::operator& (int y)               { serr(0,type); return int(realval+.5)&int(y); }
double          enigma::variant::operator& (double y)            { serr(0,type); return int(realval+.5)&int(y); }
double          enigma::variant::operator& (std::string y)       { serr(1,0); return realval; }
double          enigma::variant::operator& (char* y)             { serr(1,0); return realval; }
double          enigma::variant::operator& (enigma::variant y)   { serr(y.type,type); if (y.type==0) return int(realval+.5)&int(y.realval); return realval; }
double          enigma::variant::operator& (var& y)              { serr(y.values[0]->type,type); if (y.values[0]->type==0) return int(realval+.5)&int(y.values[0]->realval); return realval; }

double          enigma::variant::operator| (int y)               { serr(0,type); return int(realval+.5)|int(y); }
double          enigma::variant::operator| (double y)            { serr(0,type); return int(realval+.5)|int(y); }
double          enigma::variant::operator| (std::string y)       { serr(1,0); return realval; }
double          enigma::variant::operator| (char* y)             { serr(1,0); return realval; }
double          enigma::variant::operator| (enigma::variant y)   { serr(y.type,type); if (y.type==0) return int(realval+.5)|int(y.realval); return realval; }
double          enigma::variant::operator| (var& y)              { serr(y.values[0]->type,type); if (y.values[0]->type==0) return int(realval+.5)|int(y.values[0]->realval); return realval; }

double          enigma::variant::operator^ (int y)               { serr(0,type); return int(realval+.5)^int(y); }
double          enigma::variant::operator^ (double y)            { serr(0,type); return int(realval+.5)^int(y); }
double          enigma::variant::operator^ (std::string y)       { serr(1,0); return realval; }
double          enigma::variant::operator^ (char* y)             { serr(1,0); return realval; }
double          enigma::variant::operator^ (enigma::variant y)   { serr(y.type,type); if (y.type==0) return int(realval+.5)^int(y.realval); return realval; }
double          enigma::variant::operator^ (var& y)              { serr(y.values[0]->type,type); if (y.values[0]->type==0) return int(realval+.5)^int(y.values[0]->realval); return realval; }




/*******************************
 Mathematical operators for VAR
*******************************/

double          var::operator+ (int y)                             { terr(0,values[0]->type); return values[0]->realval+y; }
double          var::operator+ (double y)                          { terr(0,values[0]->type); return values[0]->realval+y; }
std::string     var::operator+ (std::string y)                     { terr(1,-1); return values[0]->stringval+y; }
std::string     var::operator+ (char* y)                           { terr(1,-1); return values[0]->stringval+y; }
enigma::variant var::operator+ (enigma::variant y)                 { terr(y.type,values[0]->type); if (y.type==0) return values[0]->realval+y.realval; return values[0]->stringval+y.stringval; }
enigma::variant var::operator+ (var& y)                            { terr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return values[0]->realval+y.values[0]->realval; return values[0]->stringval+y.values[0]->stringval; }

double          var::operator- (int y)                             { serr(0,values[0]->type); return values[0]->realval-y; }
double          var::operator- (double y)                          { serr(0,values[0]->type); return values[0]->realval-y; }
double          var::operator- (std::string y)                     { serr(1,0); return values[0]->realval; }
double          var::operator- (char* y)                           { serr(1,0); return values[0]->realval; }
double          var::operator- (enigma::variant y)                 { serr(y.type,values[0]->type); if (y.type==0) return values[0]->realval-y.realval; return values[0]->realval; }
double          var::operator- (var& y)                            { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return values[0]->realval-y.values[0]->realval; return values[0]->realval; }

double          var::operator* (int y)                             { serr(0,values[0]->type); return values[0]->realval*y; }
double          var::operator* (double y)                          { serr(0,values[0]->type); return values[0]->realval*y; }
double          var::operator* (std::string y)                     { serr(1,0); return values[0]->realval; }
double          var::operator* (char* y)                           { serr(1,0); return values[0]->realval; }
double          var::operator* (enigma::variant y)                 { serr(y.type,values[0]->type); if (y.type==0) return values[0]->realval*y.realval; return values[0]->realval; }
double          var::operator* (var& y)                            { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return values[0]->realval*y.values[0]->realval; return values[0]->realval; }

double          var::operator/ (int y)                             { serr(0,values[0]->type); return values[0]->realval/y; }
double          var::operator/ (double y)                          { serr(0,values[0]->type); return values[0]->realval/y; }
double          var::operator/ (std::string y)                     { serr(1,0); return values[0]->realval; }
double          var::operator/ (char* y)                           { serr(1,0); return values[0]->realval; }
double          var::operator/ (enigma::variant y)                 { serr(y.type,values[0]->type); if (y.type==0) return values[0]->realval/y.realval; return values[0]->realval; }
double          var::operator/ (var& y)                            { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return values[0]->realval/y.values[0]->realval; return values[0]->realval; }

double          var::operator% (int y)                             { serr(0,values[0]->type); double a=values[0]->realval/y; a-=(int)a; return a*y; }
double          var::operator% (double y)                          { serr(0,values[0]->type); double a=values[0]->realval/y; a-=(int)a; return a*y; }
double          var::operator% (std::string y)                     { serr(1,0); return values[0]->realval; }
double          var::operator% (char* y)                           { serr(1,0); return values[0]->realval; }
double          var::operator% (enigma::variant y)                 { serr(y.type,values[0]->type); if (y.type==0) { double a=values[0]->realval/y.realval; a-=(int)a; return a*y.realval; } return values[0]->realval; }
double          var::operator% (var& y)                            { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) { double a=values[0]->realval/y.values[0]->realval; a-=(int)a; return a*y.values[0]->realval; } return values[0]->realval; }

double          var::operator<< (int y)                            { serr(0,values[0]->type); return int(values[0]->realval)<<int(y); }
double          var::operator<< (double y)                         { serr(0,values[0]->type); return int(values[0]->realval)<<int(y); }
double          var::operator<< (std::string y)                    { serr(1,0); return values[0]->realval; }
double          var::operator<< (char* y)                          { serr(1,0); return values[0]->realval; }
double          var::operator<< (enigma::variant y)                { serr(y.type,values[0]->type); if (y.type==0) return int(values[0]->realval)<<int(y.realval); return values[0]->realval; }
double          var::operator<< (var& y)                           { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return int(values[0]->realval)<<int(y.values[0]->realval); return values[0]->realval; }

double          var::operator>> (int y)                            { serr(0,values[0]->type); return int(values[0]->realval)>>int(y); }
double          var::operator>> (double y)                         { serr(0,values[0]->type); return int(values[0]->realval)>>int(y); }
double          var::operator>> (std::string y)                    { serr(1,0); return values[0]->realval; }
double          var::operator>> (char* y)                          { serr(1,0); return values[0]->realval; }
double          var::operator>> (enigma::variant y)                { serr(y.type,values[0]->type); if (y.type==0) return int(values[0]->realval)>>int(y.realval); return values[0]->realval; }
double          var::operator>> (var& y)                           { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return int(values[0]->realval)>>int(y.values[0]->realval); return values[0]->realval; }







double          var::operator& (int y)                            { serr(0,values[0]->type); return int(values[0]->realval)&int(y); }
double          var::operator& (double y)                         { serr(0,values[0]->type); return int(values[0]->realval)&int(y); }
double          var::operator& (std::string y)                    { serr(1,0); return values[0]->realval; }
double          var::operator& (char* y)                          { serr(1,0); return values[0]->realval; }
double          var::operator& (enigma::variant y)                { serr(y.type,values[0]->type); if (y.type==0) return int(values[0]->realval)&int(y.realval); return values[0]->realval; }
double          var::operator& (var& y)                           { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return int(values[0]->realval)&int(y.values[0]->realval); return values[0]->realval; }

double          var::operator| (int y)                            { serr(0,values[0]->type); return int(values[0]->realval)|int(y); }
double          var::operator| (double y)                         { serr(0,values[0]->type); return int(values[0]->realval)|int(y); }
double          var::operator| (std::string y)                    { serr(1,0); return values[0]->realval; }
double          var::operator| (char* y)                          { serr(1,0); return values[0]->realval; }
double          var::operator| (enigma::variant y)                { serr(y.type,values[0]->type); if (y.type==0) return int(values[0]->realval)|int(y.realval); return values[0]->realval; }
double          var::operator| (var& y)                           { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return int(values[0]->realval)|int(y.values[0]->realval); return values[0]->realval; }

double          var::operator^ (int y)                            { serr(0,values[0]->type); return int(values[0]->realval)^int(y); }
double          var::operator^ (double y)                         { serr(0,values[0]->type); return int(values[0]->realval)^int(y); }
double          var::operator^ (std::string y)                    { serr(1,0); return values[0]->realval; }
double          var::operator^ (char* y)                          { serr(1,0); return values[0]->realval; }
double          var::operator^ (enigma::variant y)                { serr(y.type,values[0]->type); if (y.type==0) return int(values[0]->realval)^int(y.realval); return values[0]->realval; }
double          var::operator^ (var& y)                           { serr(y.values[0]->type,values[0]->type); if (y.values[0]->type==0) return int(values[0]->realval)^int(y.values[0]->realval); return values[0]->realval; }






/*********************************
 Comparison operators for VARIANT
*********************************/

bool enigma::variant::operator<  (int y)               { serr(type,0); return realval<y; }
bool enigma::variant::operator<  (double y)            { serr(type,0); return realval<y; }
bool enigma::variant::operator<  (std::string y)       { serr(0,1); return 0; }
bool enigma::variant::operator<  (char* y)             { serr(0,1); return 0; }
bool enigma::variant::operator<  (enigma::variant y)   { serr(type,y.type); return realval<y.realval; }
bool enigma::variant::operator<  (var& y)              { serr(type,y.values[0]->type); return realval<y.values[0]->realval; }

bool enigma::variant::operator>  (int y)               { serr(type,0); return realval>y; }
bool enigma::variant::operator>  (double y)            { serr(type,0); return realval>y; }
bool enigma::variant::operator>  (std::string y)       { serr(0,1); return 0; }
bool enigma::variant::operator>  (char* y)             { serr(0,1); return 0; }
bool enigma::variant::operator>  (enigma::variant y)   { serr(type,y.type); return realval>y.realval; }
bool enigma::variant::operator>  (var& y)              { serr(type,y.values[0]->type); return realval>y.values[0]->realval; }

bool enigma::variant::operator<= (int y)               { serr(type,0); return realval<=y; }
bool enigma::variant::operator<= (double y)            { serr(type,0); return realval<=y; }
bool enigma::variant::operator<= (std::string y)       { serr(0,1); return 0; }
bool enigma::variant::operator<= (char* y)             { serr(0,1); return 0; }
bool enigma::variant::operator<= (enigma::variant y)   { serr(type,y.type); return realval<=y.realval; }
bool enigma::variant::operator<= (var& y)              { serr(type,y.values[0]->type); return realval<=y.values[0]->realval; }

bool enigma::variant::operator>= (int y)               { serr(type,0); return realval>=y; }
bool enigma::variant::operator>= (double y)            { serr(type,0); return realval>=y; }
bool enigma::variant::operator>= (std::string y)       { serr(0,1); return 0; }
bool enigma::variant::operator>= (char* y)             { serr(0,1); return 0; }
bool enigma::variant::operator>= (enigma::variant y)   { serr(type,y.type); return realval>=y.realval; }
bool enigma::variant::operator>= (var& y)              { serr(type,y.values[0]->type); return realval>=y.values[0]->realval; }

bool enigma::variant::operator== (int y)               { terr(type,0); return realval==y; }
bool enigma::variant::operator== (double y)            { terr(type,0); return realval==y; }
bool enigma::variant::operator== (std::string y)       { terr(type,1); return stringval==y; }
bool enigma::variant::operator== (char* y)             { terr(type,1); return stringval==y; }
bool enigma::variant::operator== (enigma::variant y)   { terr(type,y.type); if (type==0) return realval==y.realval; return stringval==y.stringval; }
bool enigma::variant::operator== (var& y)              { terr(type,y.values[0]->type); if (type==0) return realval==y.values[0]->realval; return stringval==y.values[0]->stringval; }

bool enigma::variant::operator!= (int y)               { terr(type,0); return realval!=y; }
bool enigma::variant::operator!= (double y)            { terr(type,0); return realval!=y; }
bool enigma::variant::operator!= (std::string y)       { terr(type,1); return stringval!=y; }
bool enigma::variant::operator!= (char* y)             { terr(type,1); return stringval!=y; }
bool enigma::variant::operator!= (enigma::variant y)   { terr(type,y.type); if (type==0) return realval!=y.realval; return stringval!=y.stringval; }
bool enigma::variant::operator!= (var& y)              { terr(type,y.values[0]->type); if (type==0) return realval!=y.values[0]->realval; return stringval!=y.values[0]->stringval; }


/*****************************
 Comparison operators for VAR
*****************************/

bool var::operator<  (int y)               { serr(values[0]->type,0); return values[0]->realval<y; }
bool var::operator<  (double y)            { serr(values[0]->type,0); return values[0]->realval<y; }
bool var::operator<  (std::string y)       { serr(0,1); return 0; }
bool var::operator<  (char* y)             { serr(0,1); return 0; }
bool var::operator<  (enigma::variant y)   { serr(values[0]->type,y.type); return values[0]->realval<y.realval; }
bool var::operator<  (var& y)              { serr(values[0]->type,y.values[0]->type); return values[0]->realval<y.values[0]->realval; }

bool var::operator>  (int y)               { serr(values[0]->type,0); return values[0]->realval>y; }
bool var::operator>  (double y)            { serr(values[0]->type,0); return values[0]->realval>y; }
bool var::operator>  (std::string y)       { serr(0,1); return 0; }
bool var::operator>  (char* y)             { serr(0,1); return 0; }
bool var::operator>  (enigma::variant y)   { serr(values[0]->type,y.type); return values[0]->realval>y.realval; }
bool var::operator>  (var& y)              { serr(values[0]->type,y.values[0]->type); return values[0]->realval>y.values[0]->realval; }

bool var::operator<= (int y)               { serr(values[0]->type,0); return values[0]->realval<=y; }
bool var::operator<= (double y)            { serr(values[0]->type,0); return values[0]->realval<=y; }
bool var::operator<= (std::string y)       { serr(0,1); return 0; }
bool var::operator<= (char* y)             { serr(0,1); return 0; }
bool var::operator<= (enigma::variant y)   { serr(values[0]->type,y.type); return values[0]->realval<=y.realval; }
bool var::operator<= (var& y)              { serr(values[0]->type,y.values[0]->type); return values[0]->realval<=y.values[0]->realval; }

bool var::operator>= (int y)               { serr(values[0]->type,0); return values[0]->realval>=y; }
bool var::operator>= (double y)            { serr(values[0]->type,0); return values[0]->realval>=y; }
bool var::operator>= (std::string y)       { serr(0,1); return 0; }
bool var::operator>= (char* y)             { serr(0,1); return 0; }
bool var::operator>= (enigma::variant y)   { serr(values[0]->type,y.type); return values[0]->realval>=y.realval; }
bool var::operator>= (var& y)              { serr(values[0]->type,y.values[0]->type); return values[0]->realval>=y.values[0]->realval; }

bool var::operator== (int y)               { terr(values[0]->type,0); return values[0]->realval==y; }
bool var::operator== (double y)            { terr(values[0]->type,0); return values[0]->realval==y; }
bool var::operator== (std::string y)       { terr(values[0]->type,1); return values[0]->stringval==y; }
bool var::operator== (char* y)             { terr(values[0]->type,1); return values[0]->stringval==y; }
bool var::operator== (enigma::variant y)   { terr(values[0]->type,y.type); if (values[0]->type==0) return values[0]->realval==y.realval; return values[0]->stringval==y.stringval; }
bool var::operator== (var& y)              { terr(values[0]->type,y.values[0]->type); if (values[0]->type==0) return values[0]->realval==y.values[0]->realval; return values[0]->stringval==y.values[0]->stringval; }

bool var::operator!= (int y)               { terr(values[0]->type,0); return values[0]->realval!=y; }
bool var::operator!= (double y)            { terr(values[0]->type,0); return values[0]->realval!=y; }
bool var::operator!= (std::string y)       { terr(values[0]->type,1); return values[0]->stringval!=y; }
bool var::operator!= (char* y)             { terr(values[0]->type,1); return values[0]->stringval!=y; }
bool var::operator!= (enigma::variant y)   { terr(values[0]->type,y.type); if (values[0]->type==0) return values[0]->realval!=y.realval; return values[0]->stringval!=y.stringval; }
bool var::operator!= (var& y)              { terr(values[0]->type,y.values[0]->type); if (values[0]->type==0) return values[0]->realval!=y.values[0]->realval; return values[0]->stringval!=y.values[0]->stringval; }




/****************************
 Unary operators for VARIANT
****************************/

bool             enigma::variant::operator!() { uerr(type); return !realval; }
double           enigma::variant::operator~() { uerr(type); return ~int(realval+.5); }
enigma::variant* enigma::variant::operator*() { return this; }
double           enigma::variant::operator+() { uerr(type); return realval; }
double           enigma::variant::operator-() { uerr(type); return -realval; }


/************************
 Unary operators for VAR
*************************/

bool              var::operator!() { uerr(values[0]->type); return !values[0]->realval; }
double            var::operator~() { uerr(values[0]->type); return ~int(values[0]->realval+.5); }
enigma::variant** var::operator*() { return values; }
double            var::operator+() { uerr(values[0]->type); return values[0]->realval; }
double            var::operator-() { uerr(values[0]->type); return -(values[0]->realval); }
