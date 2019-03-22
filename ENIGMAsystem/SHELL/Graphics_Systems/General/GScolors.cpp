/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
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

#include "GScolors.h"
#include "GScolor_macros.h"

#include <math.h>

static inline int min(int x,int y){return x<y ? x:y;}
static inline int max(int x,int y){return x>y ? x:y;}
static inline int bclamp(int x){return x > 255 ? 255 : x < 0 ? 0 : x;}

namespace enigma {

unsigned char currentcolor[4] = {0,0,0,255};

} // namespace enigma

namespace enigma_user {

	int merge_color(int c1,int c2,double amount)
	{
		amount = amount > 1 ? 1 : (amount < 0 ? 0 : amount);
		return (unsigned char)(fabs(COL_GET_R(c1)+(COL_GET_R(c2)-COL_GET_R(c1))*amount))
		|      (unsigned char)(fabs(COL_GET_G(c1)+(COL_GET_G(c2)-COL_GET_G(c1))*amount))<<8
		|      (unsigned char)(fabs(COL_GET_B(c1)+(COL_GET_B(c2)-COL_GET_B(c1))*amount))<<16;
	}

	int draw_get_color(){
	  return enigma::currentcolor[0] | (enigma::currentcolor[1] << 8) | (enigma::currentcolor[2] << 16);
	}
	int draw_get_red(){return enigma::currentcolor[0];}
	int draw_get_green(){return enigma::currentcolor[1];}
	int draw_get_blue(){return enigma::currentcolor[2];}

	float draw_get_alpha(){
	  return enigma::currentcolor[3] / 255.0;
	}

	int color_get_red  (int c){return COL_GET_R(c);}
	int color_get_green(int c){return COL_GET_G(c);}
	int color_get_blue (int c){return COL_GET_B(c);}

	int color_get_hue(int c)
	{
		int r = COL_GET_R(c),g = COL_GET_G(c),b = COL_GET_B(c);
		int cmpmax = r>g ? (r>b?r:b) : (g>b?g:b);
		if(!cmpmax) return 0;

		double cmpdel = cmpmax - (r<g ? (r<b?r:b) : (g<b?g:b)); //Maximum difference
		double h = (r == cmpmax ? (g-b)/cmpdel : (g==cmpmax ? 2-(r-g)/cmpdel : 4+(r-g)/cmpdel));
		return int((h<0 ? h+6 : h) * 42.5); //42.5 = 60/360*255
	}
	int color_get_value(int c)
	{
	  int r = COL_GET_R(c), g = COL_GET_G(c), b = COL_GET_B(c);
		return r>g ? (r>b?r:b) : (g>b?g:b);
	}
	int color_get_saturation(int color)
	{
		int r = COL_GET_R(color), g = COL_GET_G(color), b = COL_GET_B(color);
		int cmpmax = r>g  ?  (r>b ? r : b)  :  (g>b ? g : b);
		return cmpmax  ?  255 - int(255 * (r<g ? (r<b?r:b) : (g<b?g:b)) / double(cmpmax))  :  0;
	}

	int make_color_rgb(unsigned char r, unsigned char g, unsigned char b){
	  return r | (g << 8) | (b << 16);
	}

	int make_color_rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
	  return r | (g << 8) | (b << 16) | (a << 24);
	}

	int make_color_hsv(int hue,int saturation,int value)
	{
	  int h = hue&255, s = saturation&255,v = value&255;
	  double vf = v; vf /= 255.0;
	  double
	    red   = bclamp(510 - min(h,     255-h) * 6) * vf,
	    green = bclamp(510 - max(85-h,   h-85) * 6) * vf,
	    blue  = bclamp(510 - max(170-h, h-170) * 6) * vf;

	  red   += (v-red)   * (1 - s/255.0);
	  green += (v-green) * (1 - s/255.0);
	  blue  += (v-blue)  * (1 - s/255.0);

	  int redr   = int(red);
	  int greenr = int(green);
	  int bluer  = int(blue);

	  return (redr>0 ? redr : 0) | (greenr>0 ? (greenr<<8) : 0) | (bluer>0 ? (bluer<<16) : 0);
	}
}
