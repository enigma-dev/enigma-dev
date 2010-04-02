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

#include <GL/gl.h>
#include <math.h>

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00)>>8)
#define __GETB(x) ((x & 0xFF0000)>>16)
#define __GETRf(x) fmod(x,256)
#define __GETGf(x) fmod(x/256,256)
#define __GETBf(x) fmod(x/65536,256)
namespace enigma{ extern float currentcolor[4];}

int draw_clear_alpha(double col,float alpha){
	glClearColor(__GETRf(col)/255,__GETGf(col)/255,__GETBf(col)/255,alpha);
	glClear(GL_COLOR_BUFFER_BIT);
	return 0;
}
int draw_clear(double col){return draw_clear_alpha(col,1);}

int merge_color(int col1, int col2, double amount)
{
  unsigned char* c1 = (unsigned char*)&col1;
  unsigned char* c2 = (unsigned char*)&col2;
  c1[0] = char(c1[0]*(1-amount) + c2[0]*(amount));
  c1[1] = char(c1[1]*(1-amount) + c2[1]*(amount));
  c1[2] = char(c1[2]*(1-amount) + c2[2]*(amount));
  c1[3] = char(c1[3]*(1-amount) + c2[3]*(amount));
  return col1;
}

int draw_set_color(double color){
	enigma::currentcolor[0]=__GETRf(color)/255;
	enigma::currentcolor[1]=__GETGf(color)/255;
	enigma::currentcolor[2]=__GETBf(color)/255;
	glColor4fv(enigma::currentcolor);
	return 0;
}
int draw_set_color_rgb(float red,float green,float blue){
	enigma::currentcolor[0]=fmod(red,256)/255;
	enigma::currentcolor[1]=fmod(green,256)/255;
	enigma::currentcolor[2]=fmod(blue,256)/255;
	glColor4fv(enigma::currentcolor);
	return 0;
}
int draw_set_alpha(float alpha){
	enigma::currentcolor[3]=alpha>1?1:(alpha<0?0:alpha);
	glColor4fv(enigma::currentcolor);
	return 0;
}
int draw_set_color_rgba(float red,float green,float blue,float alpha){
	enigma::currentcolor[0]=fmod(red,256)/255;
	enigma::currentcolor[1]=fmod(green,256)/255;
	enigma::currentcolor[2]=fmod(blue,256)/255;
	enigma::currentcolor[3]=alpha>1?1:(alpha<0?0:alpha);
	glColor4fv(enigma::currentcolor);
	return 0;
}

double draw_get_color(){return enigma::currentcolor[0]*255+enigma::currentcolor[1]*65280+enigma::currentcolor[2]*16711680;}
float draw_get_red(){return enigma::currentcolor[0];}
float draw_get_green(){return enigma::currentcolor[1];}
float draw_get_blue(){return enigma::currentcolor[2];}
float draw_get_alpha(){return enigma::currentcolor[3];}
double make_color_rgb(double r, double g, double b){return fmod(r,256)+fmod(g,256)*256+fmod(b,256)*65536;}
double color_get_red(double c){return __GETRf(c);}
double color_get_green(double c){return __GETGf(c);}
double color_get_blue(double c){return __GETBf(c);}
double color_get_hue(double c){
	double r=__GETRf(c),g=__GETGf(c),b=__GETBf(c);
	double cmpmax=r>g?(r>b?r:b):(g>b?g:b);
	if(!cmpmax) return 0;
	double cmpdel=cmpmax-(r<g?(r<b?r:b):(g<b?g:b));
	double h=(r==cmpmax?(g-b)/cmpdel:(g==cmpmax?2+(r-g)/cmpdel:4+(r-g)/cmpdel));
	return (h<0?h+6:h)*60;
}
double color_get_value(double c){
    double r=__GETRf(c),g=__GETGf(c),b=__GETBf(c);
	return r>g?(r>b?r:b):(g>b?g:b);
}
double color_get_saturation(double color){
	double r=__GETRf(color),g=__GETGf(color),b=__GETBf(color),cmpmax=r>g?(r>b?r:b):(g>b?g:b);
	return cmpmax?1-(r<g?(r<b?r:b):(g<b?g:b))/cmpmax:0;
}
double make_color_hsv(double h,double s,double v){
	h=fmod(h,256);
	s=1-fmod(s,256)/255;
	v=fmod(v,256);
	double
		r=fmax(510-(h>128?255-h:h)/42.5*255,0),
		g=fmax(510-fabs(h-85)/42.5*255,0),
		b=fmax(510-fabs(h-170)/42.5*255,0);
	r=r<256?r+((1-r/255)*v)*s:1;
	g=g<256?((1-(g<255?g/255:1))*v)*s:1;
	b=b<256?((1-(b<255?b/255:1))*v)*s:1;
	return (r>0?r:0)+(g>0?256*g:0)+(b>0?65536*b:0);
}
