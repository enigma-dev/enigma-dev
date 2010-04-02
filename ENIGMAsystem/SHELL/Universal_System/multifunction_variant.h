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

#define REAL__ double //Use double as the real type
#define STRING__ std::string //Use std::string as the string type
#define FREEOLD__ true //Free strings when switching to real, zero real when switching to string

namespace enigma
{
  struct TYPEPURPOSE:variant
  {
    double *reflex1,*reflex2,*reflex3;
    //These are assignment operators and require a reference to be passed
    double&operator= (int y),  &operator= (double y);   std::string &operator= (std::string y),  &operator= (char* y); 
    enigma::variant& 
    operator= (enigma::variant y), 
    &operator= (var& y);
    double&operator+= (int y), &operator+= (double y);  std::string &operator+= (std::string y), &operator+= (char* y);  enigma::variant &operator+= (enigma::variant y), &operator+= (var& y);
    double&operator-= (int y), &operator-= (double y);  std::string &operator-= (std::string y), &operator-= (char* y);  enigma::variant &operator-= (enigma::variant y), &operator-= (var& y);
    double&operator*= (int y), &operator*= (double y);  std::string &operator*= (std::string y), &operator*= (char* y);  enigma::variant &operator*= (enigma::variant y), &operator*= (var& y);
    double&operator/= (int y), &operator/= (double y);  std::string &operator/= (std::string y), &operator/= (char* y);  enigma::variant &operator/= (enigma::variant y), &operator/= (var& y);
    double&operator<<= (int y),&operator<<= (double y); std::string &operator<<= (std::string y),&operator<<= (char* y); enigma::variant &operator<<= (enigma::variant y),&operator<<= (var& y);
    double&operator>>= (int y),&operator>>= (double y); std::string &operator>>= (std::string y),&operator>>= (char* y); enigma::variant &operator>>= (enigma::variant y),&operator>>= (var& y);
    double&operator&= (int y), &operator&= (double y);  std::string &operator&= (std::string y), &operator&= (char* y);  enigma::variant &operator&= (enigma::variant y), &operator&= (var& y);
    double&operator|= (int y), &operator|= (double y);  std::string &operator|= (std::string y), &operator|= (char* y);  enigma::variant &operator|= (enigma::variant y), &operator|= (var& y);
    double&operator^= (int y), &operator^= (double y);  std::string &operator^= (std::string y), &operator^= (char* y);  enigma::variant &operator^= (enigma::variant y), &operator^= (var& y);
  };
}
