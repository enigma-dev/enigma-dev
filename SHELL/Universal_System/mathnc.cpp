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
#include <stdlib.h>

namespace enigma { extern int Random_Seed; }

/*
 Source
*/

double random(double n)  //Note: Algorithm based off of Delphi pseudorandom functions.
    {
        double rval=(0.031379939289763571*(     enigma::Random_Seed % 32)
               +0.00000000023283064365387*(((int)enigma::Random_Seed / 32) +1)
               +0.004158057505264878*((int)enigma::Random_Seed / 32)
               );

        enigma::Random_Seed=rand();
        return (rval-((int) rval))*n;
    }

    int random_set_seed(int seed)
    {
        enigma::Random_Seed=seed;
        return 0;
    }

    int random_get_seed()
    {
        return enigma::Random_Seed;
    }

    int randomize()
    {
        enigma::Random_Seed=rand();
        return 0;
    }

    int random_integer(double x)
    {
        return rand() % (int)x;
    }


double round(double x)
{
    return (int)(x+0.5);
}

double sqr(double x)
{
        return (double) (x*x);
}
double power(double x,double power)
{
  return pow(x,power);
}
double ln(double x)
{
  return log(x);
}
double logn(double n,double x)
{
  return log(x)/log(n);
}
double log2(double x)
{
  return log(x)/log(2);
}

double arcsin(double x)
{
  return asin(x);
}
double arccos(double x)
{
  return acos(x);
}
double arctan(double x)
{
  return atan(x);
}
double arctan2(double y,double x)
{
  return atan2(y,x);
}

double min(double value1, double value2)
{
  if (value1<value2)
  return value1;
  return value2;
}
double max(double value1, double value2)
{
  if (value1>value2)
  return value1;
  return value2;
}

int sign(double x)
{
  if (x==0)
  return 0;
  return (int)(x/fabs(x));
}

double frac(double x)
{
  return x-((int) x);
}

double degtorad(double x)
{
  return x*3.1415926535897932384626433832795/180.0;
}
double radtodeg(double x)
{
  return x*(180.0/3.1415926535897932384626433832795);
}

double lengthdir_x(double len,double dir)
{
  return len*cos(degtorad(dir));
}
double lengthdir_y(double len,double dir)
{
  return len*-sin(degtorad(dir));
}

double direction_difference(double dir1,double dir2)
{
  return ((((int)(dir2-dir1+.5)%360)+540)%360)-180; //Special thanks to C-Ator for the fixed formula
}

/*double log10(double x)
double sin(double x)
double cos(double x)
double tan(double x)*/

