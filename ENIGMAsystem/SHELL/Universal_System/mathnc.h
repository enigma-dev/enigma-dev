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

/**Standard math functions*****************************************************\

double random(double n)
int random_set_seed(int seed)
int random_get_seed()
int randomize()
int random_integer(double x)

double round(double x)
double sqr(double x)
double power(double x,double power)
double ln(double x)
double logn(double n,double x)
double log2(double x)
double arcsin(double x)
double arccos(double x)
double arctan(double x)
double arctan2(double y,double x)
double min(double value1, double value2)
double max(double value1, double value2)
int sign(double x)
double frac(double x)
double degtorad(double x)
double radtodeg(double x)
double lengthdir_x(double len,double dir)
double lengthdir_y(double len,double dir)
double direction_difference(double dir1,double dir2)

double sin(double x)
double cos(double x)
double tan(double x)
double log10(double x)
double abs(double x)
double ceil(double x)
double floor(double x)
double sqrt(double x)
double exp(double x)
point_direction
point_distance
\******************************************************************************/

namespace enigma {
	unsigned int Random_Seed;
	unsigned long mt[625];
}

extern double random(double n);
int random_set_seed(int seed);
int random_get_seed();
int randomize();
int random_integer(double x);
int random32();
double mtrandom();
int mtrandom_seed(int x);

double round(double x);
double sqr(double x);
double power(double x,double power);
double ln(double x);
double logn(double n,double x);
double log2(double x);

double arcsin(double x);
double arccos(double x);
double arctan(double x);
double arctan2(double y,double x);

double min(double value1, double value2);
double max(double value1, double value2);
int sign(double x);
double frac(double x);

double degtorad(double x);
double radtodeg(double x);

double lengthdir_x(double len,double dir);
double lengthdir_y(double len,double dir);
double direction_difference(double dir1,double dir2);
double point_direction(double x1,double y1,double x2,double y2);
double point_distance(double x1, double y1, double x2, double y2);


/*double sin(double x);
double cos(double x);
double tan(double x);
double log10(double x);
double abs(double x);
double ceil(double x);
double floor(double x);
double sqrt(double x);
double exp(double x);*/
