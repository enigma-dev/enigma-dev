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

/**Standard math functions*****************************************************\

double random(ARG n)
int random_set_seed(ARG seed)
int random_get_seed()
int randomize()
int random_integer(ARG x)
double abs(ARG x)
double ceil(ARG x)
double floor(ARG x)
double round(ARG x)
double sqr(ARG x)
double sqrt(ARG x)
double exp(ARG x)
double power(ARG x,ARG2 power)
double ln(ARG x)
double logn(ARG n,ARG2 x)
double log2(ARG x)
double log10(ARG x)
double sin(ARG x)
double cos(ARG x)
double tan(ARG x)
double arcsin(ARG x)
double arccos(ARG x)
double arctan(ARG x)
double arctan2(ARG y,ARG2 x)
double min(ARG value1, ARG2 value2)
double max(ARG value1, ARG2 value2)
int sign(ARG x)
double frac(ARG x)
double degtorad(ARG x)
double radtodeg(ARG x)
double lengthdir_x(ARG len,ARG2 dir)
double lengthdir_y(ARG len,ARG2 dir)

//Note:
///Both point_direction and point_distance are declared in EGMstd.h

\******************************************************************************/

int __ENIGMA_Random_Seed=rand();
    double random(double n)  //Note: Algorithm based off of Delphi pseudorandom functions.
    {
        double rval=(0.031379939289763571*(     __ENIGMA_Random_Seed % 32)
               +0.00000000023283064365387*(((int)__ENIGMA_Random_Seed / 32) +1)
               +0.004158057505264878*((int)__ENIGMA_Random_Seed / 32)
               );

        __ENIGMA_Random_Seed=rand();
        return (rval-((int) rval))*n;
    }

    int random_set_seed(int seed)
    {
        __ENIGMA_Random_Seed=seed;
        return 0;
    }

    int random_get_seed()
    {
        return __ENIGMA_Random_Seed;
    }

    int randomize()
    {
        __ENIGMA_Random_Seed=rand();
        return 0;
    }

	unsigned int random_integer(double x)
	{
		/*FIXME
		I've fixed the constrained short valueness of this, but it still lacks some things
		rand()%x is bad practice. It focuses on the low bits, which are risky if rand isn't well programmed
		rand()%x is bad practice. It is only weighted evenly if x is a power of tw
		*/
		return ((unsigned int)x<65536?rand():rand()<<16|rand())%(unsigned int)v;
    }
double round(double x){return (int)(x+0.5);}
double sqr(double x){return x*x;}
double power(double x,double power){return pow(x,power);}
double ln(double x){return log(x);}
double logn(double n,double x){return log(x)/log(n);}
double log2(double x){return log(x)/M_LN2;}//C99 standardized this. Should #if this
/*
double log10(double x){return std::log10(x);}
double sin(double x){return std::sin(x);}
double cos(double x){return std::cos(x);}
double tan(double x){return std::tan(x);}
*/
double arcsin(double x){return asin(x);}
double arccos(double x){return acos(x);}
double arctan(double x){return atan(x);}
double arctan2(double y,double x){return atan2(y,x);}
double min(double value1, double value2){return value1<value2?value1:value2;}
double max(double value1, double value2){return value1>value2?value1:value2;}
int sign(double x){return (x>0)-(x<0);}
double frac(double x){return x-(int)x;}
double degtorad(double x){return x/(180.0*pi);}
double radtodeg(double x){return x*(180.0/pi);}
double lengthdir_x(double len,double dir){return len*cos(degtorad(dir));}
double lengthdir_y(double len,double dir){return len*-sin(degtorad(dir));}
double direction_difference(double dir1,double dir2){
	dir1=negmod(dir2-dir1,360)+180;
	return dir1-(dir1>360?540:180);
}
