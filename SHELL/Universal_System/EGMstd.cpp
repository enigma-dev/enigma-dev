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

#include <math.h>
#include <string>
#include "var_cr3.h"
#include "EGMstd.h"


/*
Source
*/



double point_direction(double x1,double y1,double x2,double y2)
{
    return (int(180+180*(1-atan2((y2-y1),(x2-x1))/pi))) % 360;
}

double point_distance(double x1, double y1, double x2, double y2)
{
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}



std::string string(double val)
{
  char retstr[70];

  if ((int)val==val)
  sprintf(retstr,"%d",(int)val);
  else
  {
     sprintf(retstr,"%.2f",val);

     int a; for(a=0;retstr[a]!=0;a++);
     while (retstr[--a]=='0')
     { retstr[a]='\0'; }
  }

  return retstr;
}
std::string string(var& val)
{
   if (val.values[0]->type==0)
   {
     char retstr[70];

     double aa=(double)val;

     if ((int)aa==aa)
     sprintf(retstr,"%d",(int)aa);
     else
     {
     sprintf(retstr,"%.2f",aa);

     int a; for(a=0;retstr[a]!=0;a++);
     while (retstr[--a]=='0')
     { retstr[a]='\0'; }
     }

     return retstr;
   }
   else
   {
       return (std::string)val;
   }
}
std::string string(enigma::variant& val)
{
   if (val.type==0)
   {
     char retstr[70];

     double aa=(double)val;

     if ((int)aa==aa)
     sprintf(retstr,"%d",(int)aa);
     else
     {
     sprintf(retstr,"%.2f",aa);

     int a; for(a=0;retstr[a]!=0;a++);
     while (retstr[--a]=='0')
     { retstr[a]='\0'; }
     }

     return retstr;
   }
   else
   {
       return (std::string)val;
   }
}

std::string string(std::string val)
{
     return val;
}
std::string string(char* val)
{
     return val;
}



std::string string_hex(double val)
{
  char retstr[70];

  sprintf(retstr,"%X",(int)val);

  return retstr;
}
std::string string_hex(var& val)
{
   if (val.values[0]->type==0)
   {
     char retstr[70];

     int aa=(int)val;

     sprintf(retstr,"%X",aa);

     return retstr;
   }
   else
   {
       return (std::string)val;
   }
}
std::string string_hex(enigma::variant& val)
{
   if (val.type==0)
   {
     char retstr[70];

     int aa=(int)val;

     sprintf(retstr,"%X",aa);

     return retstr;
   }
   else
   {
       return (std::string)val;
   }
}

std::string string_hex(std::string val)
{
     return val;
}
std::string string_hex(char* val)
{
     return val;
}





std::string string_pointer(void* val)
{
  char retstr[70];

  sprintf(retstr,"%p",val);

  return retstr;
}

