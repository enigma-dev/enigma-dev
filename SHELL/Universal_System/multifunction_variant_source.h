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

Don't use these. Really. Unless you know what
you are doing. You could easily cause another
segmentation fault if you aren't careful. And
I say another because of how many I've had to
deal with making these types, and the rest of
ENIGMA as a whole. Don't screw it up.

*******************************************/



#define REAL__ double //Use double as the real type
#define STRING__ std::string //Use std::string as the string type
#define FREEOLD__ true //Free strings when switching to real, zero real when switching to string

namespace enigma
{
  struct TYPEPURPOSE:variant
  {
    double *reflex1, *reflex2, *reflex3;
    
    //These are assignment operators and require a reference to be passed
    double &operator= (int y),  &operator= (double y);   std::string &operator= (std::string y),  &operator= (char* y); enigma::variant& operator= (enigma::variant y), &operator= (var& y);
    double &operator+= (int y), &operator+= (double y);  std::string &operator+= (std::string y), &operator+= (char* y);  enigma::variant &operator+= (enigma::variant y), &operator+= (var& y);
    double &operator-= (int y), &operator-= (double y);  std::string &operator-= (std::string y), &operator-= (char* y);  enigma::variant &operator-= (enigma::variant y), &operator-= (var& y);
    double &operator*= (int y), &operator*= (double y);  std::string &operator*= (std::string y), &operator*= (char* y);  enigma::variant &operator*= (enigma::variant y), &operator*= (var& y);
    double &operator/= (int y), &operator/= (double y);  std::string &operator/= (std::string y), &operator/= (char* y);  enigma::variant &operator/= (enigma::variant y), &operator/= (var& y);
    double &operator<<= (int y),&operator<<= (double y); std::string &operator<<= (std::string y),&operator<<= (char* y); enigma::variant &operator<<= (enigma::variant y),&operator<<= (var& y);
    double &operator>>= (int y),&operator>>= (double y); std::string &operator>>= (std::string y),&operator>>= (char* y); enigma::variant &operator>>= (enigma::variant y),&operator>>= (var& y);
    
    double &operator&= (int y), &operator&= (double y);  std::string &operator&= (std::string y), &operator&= (char* y);  enigma::variant &operator&= (enigma::variant y), &operator&= (var& y);
    double &operator|= (int y), &operator|= (double y);  std::string &operator|= (std::string y), &operator|= (char* y);  enigma::variant &operator|= (enigma::variant y), &operator|= (var& y);
    double &operator^= (int y), &operator^= (double y);  std::string &operator^= (std::string y), &operator^= (char* y);  enigma::variant &operator^= (enigma::variant y), &operator^= (var& y);
  };
}




double          &enigma::TYPEPURPOSE::operator= (int y)                { realval=y; clearstr(); type=0; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator= (double y)             { realval=y; clearstr(); type=0; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator= (std::string y)        { stringval=y; clearreal(); type=1; TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator= (char* y)              { stringval=y; clearreal(); type=1; TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator= (enigma::variant y)    { realval=y.realval; stringval=y.stringval; type=y.type; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator= (var& y)               { realval=y.values[0]->realval; stringval=y.values[0]->stringval; type=y.values[0]->type;; TYPEFUNCTION(); return *this; }

double          &enigma::TYPEPURPOSE::operator+= (int y)               { terr(0,type); realval+=y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator+= (double y)            { terr(0,type); realval+=y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator+= (std::string y)       { terr(1,type); stringval+=y; TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator+= (char* y)             { terr(1,type); stringval+=y; TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator+= (enigma::variant y)   { terr(y.type,type); if (y.type==0) realval+=y.realval; else stringval+=y.stringval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator+= (var& y)              { terr(y.values[0]->type,type); if (y.values[0]->type==0) realval+=y.values[0]->realval; else stringval+=y.values[0]->stringval; TYPEFUNCTION(); return *this; }

double          &enigma::TYPEPURPOSE::operator-= (int y)               { terr(0,type); realval-=y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator-= (double y)            { terr(0,type); realval-=y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator-= (std::string y)       { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator-= (char* y)             { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator-= (enigma::variant y)   { terr(y.type,type); realval-=y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator-= (var& y)              { terr(y.values[0]->type,type); realval-=y.values[0]->realval; TYPEFUNCTION(); return *this; }

double          &enigma::TYPEPURPOSE::operator*= (int y)               { terr(0,type); realval*=y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator*= (double y)            { terr(0,type); realval*=y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator*= (std::string y)       { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator*= (char* y)             { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator*= (enigma::variant y)   { terr(y.type,type); realval*=y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator*= (var& y)              { terr(y.values[0]->type,type); realval*=y.values[0]->realval; TYPEFUNCTION(); return *this; }                           

double          &enigma::TYPEPURPOSE::operator/= (int y)               { terr(0,type); realval/=y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator/= (double y)            { terr(0,type); realval/=y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator/= (std::string y)       { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator/= (char* y)             { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator/= (enigma::variant y)   { terr(y.type,type); realval/=y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator/= (var& y)              { terr(y.values[0]->type,type); realval/=y.values[0]->realval; TYPEFUNCTION(); return *this; }

double          &enigma::TYPEPURPOSE::operator<<= (int y)              { terr(0,type); realval=(int)realval<<y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator<<= (double y)           { terr(0,type); realval=int(realval+.5)<<(int)y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator<<= (std::string y)      { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator<<= (char* y)            { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator<<= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)<<(int)y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator<<= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)<<(int)y.values[0]->realval; TYPEFUNCTION(); return *this; }

double          &enigma::TYPEPURPOSE::operator>>= (int y)              { terr(0,type); realval=int(realval+.5)>>y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator>>= (double y)           { terr(0,type); realval=int(realval+.5)>>(int)y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator>>= (std::string y)      { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator>>= (char* y)            { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator>>= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)>>(int)y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator>>= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)>>(int)y.values[0]->realval; TYPEFUNCTION(); return *this; }




double          &enigma::TYPEPURPOSE::operator&= (int y)              { terr(0,type); realval=(int)realval&y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator&= (double y)           { terr(0,type); realval=int(realval+.5)&(int)y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator&= (std::string y)      { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator&= (char* y)            { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator&= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)&(int)y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator&= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)&(int)y.values[0]->realval; TYPEFUNCTION(); return *this; }

double          &enigma::TYPEPURPOSE::operator|= (int y)              { terr(0,type); realval=(int)realval|y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator|= (double y)           { terr(0,type); realval=int(realval+.5)|(int)y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator|= (std::string y)      { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator|= (char* y)            { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator|= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)|(int)y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator|= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)|(int)y.values[0]->realval; TYPEFUNCTION(); return *this; }

double          &enigma::TYPEPURPOSE::operator^= (int y)              { terr(0,type); realval=(int)realval^y; TYPEFUNCTION(); return realval; }
double          &enigma::TYPEPURPOSE::operator^= (double y)           { terr(0,type); realval=int(realval+.5)^(int)y; TYPEFUNCTION(); return realval; }
std::string     &enigma::TYPEPURPOSE::operator^= (std::string y)      { terr(0,1); TYPEFUNCTION(); return stringval; }
std::string     &enigma::TYPEPURPOSE::operator^= (char* y)            { terr(0,1); TYPEFUNCTION(); return stringval; }
enigma::variant &enigma::TYPEPURPOSE::operator^= (enigma::variant y)  { terr(y.type,type); realval=int(realval+.5)^(int)y.realval; TYPEFUNCTION(); return *this; }
enigma::variant &enigma::TYPEPURPOSE::operator^= (var& y)             { terr(y.values[0]->type,type); realval=int(realval+.5)^(int)y.values[0]->realval; TYPEFUNCTION(); return *this; }

